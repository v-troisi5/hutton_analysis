import random
import math
from collections import defaultdict

CIPHERTEXT = "BSDULXXUPXKHLZXVJRKOQZVXUVOGQHZAGHSSELJXQSKHVLHMBEDGCCAXNAYIAZGFNMVFFEEUFPGIYEIDZTPJYNNBLGSNXUYVTWYTIFLUOJTNRTTPLBDEUPJMVMNMWAQRMTTQPLBBOGDLUIUJTXZIZXUOBDPDABMNNUJJAMUYDDDCXWYFHDHWPNUALTPYYMKVTGJIXFBMXMGIWNZEHDUAFCWZKIIHQIQVEWKIEFILDPEBSGNUOCFDQMEWXXTLJVS"   # sostituire con il ciphertext reale
KW1_LENGTHS = [3, 4, 5, 6, 7, 8]
KW2_LENGTH_MIN = 3
KW2_LENGTH_MAX = 10

RESTARTS_PER_PAIR = 10
T_START = 10.0
T_END = 0.1
COOLING = 0.97
STEPS_PER_TEMP = 200
PLATEAU_THRESHOLD = 100

ALPHABET = "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
ALPHABET_LIST = list(ALPHABET)
ALPHABET_SET = set(ALPHABET)

TETRAGRAMS = {}
TETRAGRAMS_FLOOR = 0.0

# Tetragrams
def load_tetragrams(filename):
    global TETRAGRAMS, TETRAGRAMS_FLOOR

    print("Loading tetragrams...\n")
    total, counts = 0, {}

    with open(filename) as f:
        for line in f:
            gram, count = line.split()
            count = int(count)
            counts[gram] = count
            total += count

    TETRAGRAMS = {g: math.log10(c / total) for g, c in counts.items()}
    TETRAGRAMS_FLOOR = math.log10(0.01 / total)

    print("Tetragrams loaded.\n")


# Hutton
def make_cipher_alphabet(kw2):
    seen = []

    for ch in kw2.upper():
        if ch in ALPHABET_SET and ch not in seen:
            seen.append(ch)

    for ch in ALPHABET:
        if ch not in seen:
            seen.append(ch)

    return seen


def hutton_decrypt(ciphertext, kw1, kw2):
    ct = ciphertext.upper()
    ca = make_cipher_alphabet(kw2)
    kw1u = kw1.upper()

    result = []

    for i, ct_ch in enumerate(ct):
        k1_ch = kw1u[i % len(kw1u)]
        shift_enc = (ALPHABET.index(k1_ch) + 1)
        idx_swap = ca.index(ct_ch)
        idx_pt = (idx_swap - shift_enc) % 26

        result.append(ca[idx_pt])
        ca[idx_pt], ca[idx_swap] = ca[idx_swap], ca[idx_pt]

    return "".join(result)


# Fitness
def fitness(kw1, kw2):
    text = hutton_decrypt(CIPHERTEXT, kw1, kw2)
    n = len(text)

    if n < 4:
        return TETRAGRAMS_FLOOR
    
    return sum(
        TETRAGRAMS.get(text[i:i+4], TETRAGRAMS_FLOOR)
        for i in range(n - 3)
    ) / (n - 3)



# Perturbing Keys
def perturb_kw1(kw1):
    idx = random.randrange(len(kw1))
    new_ch = random.choice(ALPHABET_LIST)

    return kw1[:idx] + new_ch + kw1[idx+1:]


def perturb_kw2(kw2):
    idx = random.randrange(len(kw2))
    new_ch = random.choice(ALPHABET_LIST)

    return kw2[:idx] + new_ch + kw2[idx+1:]


# Key Randomization
def random_kw(length):
    return "".join(random.choices(ALPHABET_LIST, k=length))


# Main Annealing Function
def sa_run(kw1_len, kw2_len) -> tuple[str, str, float]:
    kw1 = random_kw(kw1_len)
    kw2 = random_kw(kw2_len)

    cur_score = fitness(kw1, kw2)
    best_kw1, best_kw2 = kw1, kw2
    best_score = cur_score

    T = T_START
    optimizing_kw1 = True
    plateau_count = 0

    while T > T_END:
        for _ in range(STEPS_PER_TEMP):
            if optimizing_kw1:
                new_kw1, new_kw2 = perturb_kw1(kw1), kw2
            else:
                new_kw1, new_kw2 = kw1, perturb_kw2(kw2)

            new_score = fitness(new_kw1, new_kw2)
            delta = new_score - cur_score

            if delta > 0 or random.random() < math.exp(delta / T):
                kw1, kw2 = new_kw1, new_kw2
                cur_score = new_score

            if cur_score > best_score:
                best_kw1, best_kw2 = kw1, kw2
                best_score = cur_score
                plateau_count = 0
            else:
                plateau_count += 1

            if plateau_count >= PLATEAU_THRESHOLD:
                optimizing_kw1 = not optimizing_kw1
                plateau_count = 0

        T *= COOLING

    return best_kw1, best_kw2, best_score


def run():
    results = defaultdict(list)
    best_keys = {}

    kw2_lengths = list(range(KW2_LENGTH_MIN, KW2_LENGTH_MAX + 1))
    total_pairs = len(KW1_LENGTHS) * len(kw2_lengths)
    done = 0

    for kw1_len in KW1_LENGTHS:
        for kw2_len in kw2_lengths:
            done += 1
            pair_scores = []
            pair_best = None

            for r in range(1, RESTARTS_PER_PAIR + 1):
                kw1, kw2, score = sa_run(kw1_len, kw2_len)
                pair_scores.append(score)

                if pair_best is None or score > pair_best[2]:
                    pair_best = (kw1, kw2, score)

                print(
                    f"[{done}/{total_pairs}] "
                    f"kw1_len={kw1_len} kw2_len={kw2_len} "
                    f"restart={r}/{RESTARTS_PER_PAIR} "
                    f"score={score:.6f} "
                    f"kw1={kw1} kw2={kw2} "
                    f"plaintext={hutton_decrypt(CIPHERTEXT, kw1, kw2)}"
                )

            avg_score = sum(pair_scores) / len(pair_scores)
            results[(kw1_len, kw2_len)] = pair_scores
            best_keys[(kw1_len, kw2_len)] = pair_best

            print(
                f">> Avg. Score kw1_len={kw1_len} kw2_len={kw2_len}: {avg_score:.6f}\n"
            )

    print("\n -" * 70)
    print("Ordered kw2 lengths by avg. fitness:")
    print("-" * 70)

    for kw1_len in KW1_LENGTHS:
        ranking = sorted(
            kw2_lengths,
            key=lambda l: sum(results[(kw1_len, l)]) / len(results[(kw1_len, l)]),
            reverse=True
        )

        print(f"\nkw1_len = {kw1_len}")

        for rank, kw2_len in enumerate(ranking, 1):
            avg = sum(results[(kw1_len, kw2_len)]) / len(results[(kw1_len, kw2_len)])
            best = best_keys[(kw1_len, kw2_len)]
            pt = hutton_decrypt(CIPHERTEXT, best[0], best[1])

            print(
                f"#{rank} "
                f"kw2_len={kw2_len} "
                f"Avg={avg:.6f} "
                f"Best={best[2]:.6f} "
                f"kw1={best[0]} "
                f"kw2={best[1]} "
                f"Plaintext={pt}"
            )

    # Overall best
    best_pair = max(best_keys, key=lambda k: best_keys[k][2])
    best_entry = best_keys[best_pair]

    print("\n" + "-" * 70)
    print("Overall best")
    print(f"kw1_len: {best_pair[0]}")
    print(f"kw2_len: {best_pair[1]}")
    print(f"kw1: {best_entry[0]}")
    print(f"kw2: {best_entry[1]}")
    print(f"Score: {best_entry[2]:.6f}")
    print(f"Plaintext: {hutton_decrypt(CIPHERTEXT, best_entry[0], best_entry[1])}")
    print("=" * 70)


if __name__ == "__main__":
    load_tetragrams("english_quadgrams.txt")
    run()
