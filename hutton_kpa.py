import math
from collections import defaultdict

CIPHERTEXT = "BSDULXXUPXKHLZXVJRKOQZVXUVOGQHZAGHSSELJXQSKHVLHMBEDGCCAXNAYIAZGFNMVFFEEUFPGIYEIDZTPJYNNBLGSNXUYVTWYTIFLUOJTNRTTPLBDEUPJMVMNMWAQRMTTQPLBBOGDLUIUJTXZIZXUOBDPDABMNNUJJAMUYDDDCXWYFHDHWPNUALTPYYMKVTGJIXFBMXMGIWNZEHDUAFCWZKIIHQIQVEWKIEFILDPEBSGNUOCFDQMEWXXTLJVS"
CRIB = "WITHINENGLANDOFMYHEARTINTHEWHOLEBREAD"
PERIOD = 4

# Circular mean/std utils
def circular_mean(values, modulus=26):
    angles = [2 * math.pi * v / modulus for v in values]
    s = sum(math.sin(a) for a in angles) / len(angles)
    c = sum(math.cos(a) for a in angles) / len(angles)
    mean = math.atan2(s, c) * modulus / (2 * math.pi)

    return mean % modulus

def circular_std(values, modulus=26):
    if len(values) < 2:
        return float('nan')
    
    angles = [2 * math.pi * v / modulus for v in values]
    s = sum(math.sin(a) for a in angles) / len(angles)
    c = sum(math.cos(a) for a in angles) / len(angles)

    R = math.sqrt(s**2 + c**2)
    R = min(R, 1.0 - 1e-12)

    return math.sqrt(-2 * math.log(R)) * modulus / (2 * math.pi)


# Accumulates (c_i - c_j - m_i + m_j) mod 26 for all i,j - grouped by (i mod t, j mod t), where t is the period
def estimate_kw1_diffs(ciphertext, crib, period):
    obs = defaultdict(list)
    n = len(crib)

    for i in range(n):
        m_i = ord(crib[i]) - ord('A')
        c_i = ord(ciphertext[i]) - ord('A')
        r1 = i % period
        for j in range(i + 1, n):
            m_j = ord(crib[j]) - ord('A')
            c_j = ord(ciphertext[j]) - ord('A')
            r2 = j % period
            obs[(r1, r2)].append((c_i - c_j - m_i + m_j) % 26)

    results = {}

    for pair, vals in obs.items():
        m = circular_mean(vals)
        results[pair] = {
            'n': len(vals),
            'mean': m,
            'std': circular_std(vals),
            'rounded': round(m) % 26,
        }

    return results


if __name__ == "__main__":
    ct = "".join(c for c in CIPHERTEXT.upper() if c.isalpha())
    crib = "".join(c for c in CRIB.upper() if c.isalpha())

    print("-" * 50)
    print(" Hutton v1 - Estimate kw1 differences ")
    print("-" * 50)
    print(f"Ciphertext (cut to 50 chars.) ({len(ct):4d}): {ct[:50]}...")
    print(f"Crib (cut to 50 chars.) ({len(crib):4d}): {crib[:50]}...")
    print(f"Period t: {PERIOD}\n")

    results = estimate_kw1_diffs(ct, crib, PERIOD)

    print(f"{'(i,j)'} \t {'n'} \t {'avg.'} \t {'std.'} \t {'rounded'}")
    print(f"{'-'*48}")

    for r1 in range(PERIOD):
        for r2 in range(PERIOD):
            if r1 == r2:
                continue
            d = results.get((r1, r2))
            if d:
                print(f"({r1},{r2}) \t {d['n']} \t {d['mean']:.2f} \t {d['std']:.2f} \t {d['rounded']}")

    print()
    print(f"\nShift differences p_k - p_j [= mean(i,j) - mean(i,k), in order to delete the additive constant]")
    print(f"{'pivot i'}  {'diff.'} \t\t {'estim.'} {'rounded'}")
    print(f"{'-'*48}")

    for i in range(PERIOD):
        for j in range(PERIOD):
            if j == i: 
                continue

            for k in range(PERIOD):
                if k == i or k == j: 
                    continue

                d_ij = results.get((i, j))
                d_ik = results.get((i, k))

                if d_ij and d_ik:
                    diff = (d_ij['mean'] - d_ik['mean']) % 26
                    print(f"i={i} \t p[{k}]-p[{j}] \t {diff:.2f} \t {round(diff) % 26:}")

    print("-" * 50)
