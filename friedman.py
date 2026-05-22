import math
from collections import Counter

CIPHERTEXT = "BSDULXXUPXKHLZXVJRKOQZVXUVOGQHZAGHSSELJXQSKHVLHMBEDGCCAXNAYIAZGFNMVFFEEUFPGIYEIDZTPJYNNBLGSNXUYVTWYTIFLUOJTNRTTPLBDEUPJMVMNMWAQRMTTQPLBBOGDLUIUJTXZIZXUOBDPDABMNNUJJAMUYDDDCXWYFHDHWPNUALTPYYMKVTGJIXFBMXMGIWNZEHDUAFCWZKIIHQIQVEWKIEFILDPEBSGNUOCFDQMEWXXTLJVS"
WINDOW = 42
MAX_KEY_LEN = 20
TOP = 10
IC_ENGLISH = 0.0667

def clean(text):
    return ''.join(c.upper() for c in text if c.isalpha())


def index_of_coincidence(text):
    n = len(text)

    if n < 2:
        return 0.0
    
    counts = Counter(text)
    return sum(c * (c - 1) for c in counts.values()) / (n * (n - 1))


def avg_ic_for_key_len(text, key_len):
    substrings = [text[i::key_len] for i in range(key_len)]
    ics = [index_of_coincidence(s) for s in substrings if len(s) >= 2]

    return sum(ics) / len(ics) if ics else 0.0


def friedman_contribution(ic):
    return 1.0 / (1.0 + abs(ic - IC_ENGLISH))


def analyze(text) -> tuple[list[dict], dict[int, float], list[int]]:
    n = len(text)

    if WINDOW > 0:
        n_blocks = max(1, round(n / WINDOW))
        true_window = math.ceil(n / n_blocks)
        blocks = [text[i:i + true_window] for i in range(0, n, true_window)]
    else:
        blocks = [text]

    block_lens = [len(b) for b in blocks]
    blocks_results = []
    total_scores = {k: 0.0 for k in range(2, MAX_KEY_LEN + 1)}

    for block in blocks:
        block_data = {}
        max_kl = min(MAX_KEY_LEN, len(block) // 2)

        for kl in range(2, max_kl + 1):
            ic = avg_ic_for_key_len(block, kl)
            contrib = friedman_contribution(ic)
            block_data[kl] = (ic, contrib)
            total_scores[kl] += contrib

        blocks_results.append(block_data)

    return blocks_results, total_scores, block_lens


def print_report(text):
    blocks_results, total_scores, block_lens = analyze(text)

    n = len(text)
    n_blk = len(blocks_results)

    print("-" * 50)
    print("Friedman-like Test for Key Length")
    print("-" * 50)
    print(f"Ciphertext length: {n}")
    print(f"Blocks: {n_blk} (of roughly {block_lens[0]} characters each)")
    print(f"Reference IC: {IC_ENGLISH:.4f} \n")

    pos = 0

    for b_idx, (block_data, blen) in enumerate(zip(blocks_results, block_lens)):
        start = pos
        end = pos + blen
        ranked = sorted(block_data.items(), key=lambda x: x[1][1], reverse=True)

        print(f"Block {b_idx + 1} (positions {start + 1} to {end})")
        print(f"{'Key Len.'}   {'Avg. IC'}\t{'Contribution'}")
        print("-" * 40)

        for kl, (ic, contrib) in ranked[:TOP]:
            print(f"{kl} \t   {ic:.4f} \t  {contrib:.4f}\n")

        pos = end

    ranked_total = sorted(total_scores.items(), key=lambda x: x[1], reverse=True)

    print("-" * 50)
    print("Aggregate Ranking Results")
    print("-" * 50)
    print(f"{'#'}    {'Key Len.'}    {'Score'}")
    print("-" * 40)

    for pos_r, (kl, sc) in enumerate(ranked_total[:TOP], 1):
        print(f"{pos_r} \t{kl} \t {sc:.4f}\n")


if __name__ == "__main__":
    print_report(clean(CIPHERTEXT))
