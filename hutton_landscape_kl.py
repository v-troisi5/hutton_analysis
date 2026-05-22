import math
import random
import numpy as np
import matplotlib.pyplot as plt
from collections import Counter
from sklearn.decomposition import PCA
from sklearn.metrics import mutual_info_score

CIPHERTEXT = "RXPOGDTBZJABJJBYQIVLKSBQOLXPDDNKYUOFHAVZBXWMYRISWFIMYYYLNILUCLPPCBKAMAWXRDBMQTKTGEVAZSVWNCBFEXDGINSFCPXZWVQSSTILPZEOAURZZTKEQUMGCGGSVQCGRFOCAEGLJZFCDXETBYWTRQKOFCEPEGIXLFRTKPIKDTXJYTJSCPBLBLFYLXVSOTQOBMKCMVOIYHYUEWYZNSBCCOJBMGHGBSVNNSXFZEJHRQZUTPLAMHKBPPXOHCJJRJBXKQYCVCKAQHEMOHYUOEZRRTEZHLDQXDSKNGLIAJIJKYJMPGPDYOJPAZOIFTMJGADYMJGRTTDDHATIIYBYCYLWFCNWVSESHUXEXJAOLVATTWLVRHXQTFAWOCUXXKTCMWNLTAIYHKUTLJZCPVRYDWWOELISVEFJXBBTVUKQXOYRSLGUCFWXSRGISJIGDNDAYLKDBJTCBYOJVADBNSFBWXIOZCDBPAYCBAOVZOPIYXGWCQFJOXYRGKEHGZOCDSDXRLDWVQNPSMTKFFJTIBORUAKAKMNPTYPVFWBITODDRBLEJGMSFPKVYTDJIBQFJMXUCLCWFXBBIROYMIOLWDUFGYMJFKOFADEUNLQOSWIZBCCVLJQGANICANOAAHVWARKRFFPTDZSATHNYBAWHTHLLXBYINHDRSBXEKZKLAPCPNYINWEMGUFODDFRLGTJWQDNTZJOJLYZVCZZPZWPUVVIBPTXUSMLIYIDUGFSSNVFVHAIZUUAMKXWVBIZFTULUSPBUKQDQHUVNEKKBPFFPKAFNYQUCZDJYSQGXQFBWKBZWPNOBDAJUBPPFBPABGQLQZNSJUTZYKHHITBDVQWZSQRQMSFXJFBWMXKREYHPCREOOKJVUKYIWYBZEEBPIRJPGGHMVOWEUZBYHTMPXSAVIPADIFBDYIDTKWVMTCPLJBIUCHTLJFWGZDLTFIYDEMNWTNHXUUWLYPWYIQLJDFDGYSRKFOFMUYIIGURVLDSJEZBJXCAUORBZBKWQNAAFAOUZRXHIVNOAAFAGUUVMRSJLRJTZHHKCOMXLXSBLKRBZUGCIGYLMNESWMHDEFNCWJMQKPZCBXQGYSSVPGRRLEKPJIOWIFIHLTRROHGZPMWLIFEXTPXHGHROGIEXUCXAOLMEJBWTSIATTMXPXWNOFDEFXKFMKVPXQSVWOJCFBAWMDGWUEKQJOLNEWZGSTWAWBHCRRIDAMSSWAQCFMHITIVUVTXHPRUCVXATTIFSCLRRTIJPJAPYCPOKRYJDHKILNLPNUGCCHTHLEYLWZGOQVGISDKQFYUHHJSKXKOJGRUFATWDXBQISIUOLUVPOOTXWQOGUMTVZVLLLPCCLLDEGMUFXBCRVKWUVSOKRIXVHYFJRFWVVSQJRWCVOVBXPJZPYEBSTNVHGUEJBUOYAOBUFEVLBELNXTJKUWPGNIIIKBDKVWWQKPFAWCUFROABPNMTDEGDRMNWOTIBBCTKULXFEVCHRLGBEBFEVNWZDMGSQVKCVLKWLPZHPHOYERJRLYWAYJMODAAMPDTNQCDAVBLBJTDQEJSFXAGSPTLAFVHJYHZGSBVSITWJEGCJCGUVGKCOXEWCSVCLSHCMIDYRAHIHZRAJTTJSHWZGHGXDKTQOPBBJIDYGSPILSHBNUVIGNTYKOSPEAJBLOSBYYIEFRSVCLSVFYDDKVUBWLGJKCTDINVCZUFWXVEJUSWZTGJNFACSDVGOPDAVTULPXRXONVVRDQPRTSLVBOWXAGGGEXKKZVLBEDOKBWTAPHKALBWCIYGQJKGEZPTAMQCTOLMHSRQGNMKSFZSFMHXWKTNXBCFVZTHTLQCBPFJZTKPRPJLYEDWNTWXVXLVJALGGGFDPPDTBYYRQNRUBQJXTEMGYQMMTTTVEEXWHOLQCSEKOGRBZOPENREVMTNFFQPHHZMDHQHVHPXUVHKMYMRQZQICJASZGOHUDMTJLYAKOUCLJDIKZJTUTPUHUIVMCZSZQHHSBWUOCRKLYLJGAUVKLERBZLXGJRANIHCQYQUPDYGUMBPDQJMAJMFKHELDQNGWHIABRAFLYRSUIGSVKXZTEBWONDQFRRCCIATUWGNXZVKMEXVPPPOUJFKFGSXSAQWNUNMDKFINWHXBUAEAWKUVQMTXFBSHSFKJSSSXQMSQNTOCETUGZINVAHQAYAZQTCLVYHGKRPCXPAQOKNWGRIMQBEDHLKLZDZQGLQITGICRASCNRBABUBZWRNSWCBLOHJFKLMBDLFAVAIJAOTKJUEQPXJUIIGRPXREKOVONUOAYCFRQVBTVNUTQIGEKFCTHGMDMXIIYAWNFKCNVCNNYXDQVGBNMRCHYHMVEOOMIBJWGNZIHBJMBXCWBSXDJJJVUOCNWFMAJYUXWZSRDQMLEPYKRYIUFKBHZAPWHGEPHKHYNEQGQMOWDASQFLDSIAYODRKAMTSEXSLQLNHJBAXDBDESNHFZLVCRYMJKXMRXVEVMUWCDUMEOXFJACHNYKUBQFHWEXFKBHYUCMJGQUYXMBGBGKLSTFUBVXCCKLZCVJJBUGAERMQNUQSATDYMWVOCEBKFOSHEAEZFBFZFJKYNWTUVKDHPJKKRSCCNGTQQHRRGIWZLCFGNUOAHPALRJKZIDLSSAXWRPLJCPVPHPBRNBFUMCHMOXJVMEQHCNCBNXMJRABVFOFYHMJAKGQPPBFOEWLCAXXMYYQVIGXVQFHMDASWUGOWTGOTQRBCZYQTWURVIXKCDYTIHRFJFKJEVJXYEOXCWFUWEOAYLGOFWPDWFHEGVPJALPRAQWMWMXKEWXYHCMNBZGNRFCIJQLOFCXHWFSTRGPIKMOXYWUILNVUXPUTZTKHNIAMNMLTBXWYBDWMZCQMYPJLNTBDPCWFFCHOTPLYVDVZXGITCGQPGXAVMEPWHHUAZJGOCHLYJIGMBFFFKKDNXVZVPOQCLHSX"
KW1_LENGTH = 4
KW2_LENGTH = 5
BEST_KW1 = "HILL"
BEST_KW2 = "VALEY"
ANALYZE = "both"

# Keys to sample for PCA+FDC and MI analysis
N_SAMPLES = 20000

# Random walk parameters
RW_STEPS = 2000
RW_N_WALKS = 50
RW_MAX_LAG = 50

# Mutual information bins
MI_BINS = 80

# Samples for fitness differential between characters
DIFF_SAMPLES = 5000

# Top-N quadgrams to consider
KL_TOP_N = 10000

# Quadgram smoothing
P_FLOOR = 1e-9

# IC Weight for fitness
IC_BONUS_WEIGHT = 0.0

ENGLISH_IC = 0.0667
ALPHABET = "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
ALPHABET_LIST = list(ALPHABET)
ALPHABET_SET = set(ALPHABET)
KEY_LENGTH = KW1_LENGTH + KW2_LENGTH

# Quadgrams
TETRAGRAMS = {}
P_ENGLISH = {}


def load_tetragrams(filename):
    global TETRAGRAMS, P_ENGLISH
    print("Loading tetragrams...")
    total, counts = 0, {}
    
    with open(filename) as f:
        for line in f:
            gram, count = line.split()
            count = int(count)
            counts[gram] = count
            total += count

    TETRAGRAMS = {g: math.log10(c / total) for g, c in counts.items()}

    top = sorted(counts.items(), key=lambda x: x[1], reverse=True)

    if KL_TOP_N > 0:
        top = top[:KL_TOP_N]

    ref_total = sum(c for _, c in top)
    P_ENGLISH = {g: c / ref_total for g, c in top}

    print(f"Tetragrams loaded. Top {len(P_ENGLISH)} grams.\n")


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


def hutton_decrypt_v1(ciphertext, kw1, kw2):
    ct = ciphertext.upper()
    ca = make_cipher_alphabet(kw2)
    kw1u = kw1.upper()
    result = []

    for i, ct_ch in enumerate(ct):
        k1_ch = kw1u[i % len(kw1u)]
        shift_enc = ALPHABET.index(k1_ch) + 1
        idx_swap = ca.index(ct_ch)
        idx_pt = (idx_swap - shift_enc) % 26

        result.append(ca[idx_pt])
        ca[idx_pt], ca[idx_swap] = ca[idx_swap], ca[idx_pt]

    return "".join(result)


# Decrypts and returns cumulative -KL+IC fitness for each character (from 4th onward)
# Used by plot_round_correlation() only
def hutton_decrypt_round_scores(ciphertext, kw1, kw2):
    ct = ciphertext.upper()
    ca = make_cipher_alphabet(kw2)
    kw1u = kw1.upper()
    buf = []
    scores = []

    for i, ct_ch in enumerate(ct):
        k1_ch = kw1u[i % len(kw1u)]
        shift_enc = ALPHABET.index(k1_ch) + 1
        idx_swap = ca.index(ct_ch)
        idx_pt = (idx_swap - shift_enc) % 26
        buf.append(ca[idx_pt])
        ca[idx_pt], ca[idx_swap] = ca[idx_swap], ca[idx_pt]

        if len(buf) >= 4:
            text = "".join(buf)
            n = len(text)
            obs = Counter(text[j:j+4] for j in range(n - 3))
            total = sum(obs.values())
            kl = 0.0

            # KL Divergence
            for gram, p in P_ENGLISH.items():
                q = obs.get(gram, 0) / total if total > 0 else P_FLOOR
                if q <= 0:
                    q = P_FLOOR
                kl += p * math.log(p / q)

            freq = Counter(text)
            ic = sum(f*(f-1) for f in freq.values()) / (n*(n-1))
            s = (1.0 - IC_BONUS_WEIGHT)*(-kl) + IC_BONUS_WEIGHT*(-abs(ic - ENGLISH_IC))
            scores.append(s)

    return scores


# Fitness: -KL+IC
def fitness(kw1, kw2):
    text = hutton_decrypt_v1(CIPHERTEXT, kw1, kw2)
    n = len(text)
    if n < 4:
        return -1e9

    # Observed distribution of quadgrams in ciphertext
    obs = Counter(text[i:i+4] for i in range(n - 3))
    total = sum(obs.values())

    # KL divergence computation
    kl = 0.0
    for gram, p in P_ENGLISH.items():
        q = obs.get(gram, 0) / total if total > 0 else P_FLOOR
        if q <= 0:
            q = P_FLOOR
        kl += p * math.log(p / q)

    freq = Counter(text)
    ic = sum(f * (f-1) for f in freq.values()) / (n * (n-1))

    return (1.0 - IC_BONUS_WEIGHT) * (-kl) + IC_BONUS_WEIGHT * (-abs(ic - ENGLISH_IC))


# Key utilities
def encode_key(kw1, kw2):
    key = (kw1 + kw2).upper()
    return np.array([ALPHABET.index(ch) for ch in key], dtype=float)


# Hamming distance between two keys
def hamming(kw1_a, kw2_a, kw1_b, kw2_b):
    a = (kw1_a + kw2_a).upper()
    b = (kw1_b + kw2_b).upper()

    return sum(ca != cb for ca, cb in zip(a, b))


def random_key():
    kw1 = "".join(random.choices(ALPHABET_LIST, k=KW1_LENGTH))
    kw2 = "".join(random.choices(ALPHABET_LIST, k=KW2_LENGTH))

    return kw1, kw2


def perturb(kw1, kw2, target):
    which_key = target if target != "both" else random.choice(["kw1", "kw2"])

    if which_key == "kw1":
        pos = random.randrange(KW1_LENGTH)
        new_ch = random.choice(ALPHABET_LIST)
        return kw1[:pos] + new_ch + kw1[pos+1:], kw2
    else:
        pos = random.randrange(KW2_LENGTH)
        new_ch = random.choice(ALPHABET_LIST)
        return kw1, kw2[:pos] + new_ch + kw2[pos+1:]


def sample_keys(n):
    return [random_key() for _ in range(n)]


# Principal Component Analysis on encoded keys: finds the maximum variance dimensions in the |kw1|+|kw2|-dimensional key space
# Fitness Distance Correlation: measures the correlation between fitness(kw1,kw2) and the Hamming distance between candidate kw1+kw2 and the true key
def plot_pca_fdc(keys: list[tuple[str, str]], scores: list[float]):
    print("PCA projection...")
    X = np.array([encode_key(k1, k2) for k1, k2 in keys])
    pca = PCA(n_components=2)
    X_2d = pca.fit_transform(X)
    var = pca.explained_variance_ratio_
    best_2d = pca.transform(encode_key(BEST_KW1, BEST_KW2).reshape(1, -1))[0]
    sa = np.array(scores)

    distances = np.array([hamming(k1, k2, BEST_KW1, BEST_KW2) for k1, k2 in keys], float)
    pearson_r = np.corrcoef(distances, sa)[0, 1]

    fig, axes = plt.subplots(1, 2, figsize=(15, 6))
    fig.suptitle(f"PCA + FDC | ANALYZE={ANALYZE} kw1={BEST_KW1} kw2={BEST_KW2}", fontsize=11)

    ax = axes[0]
    sc = ax.scatter(X_2d[:, 0], X_2d[:, 1], c=sa, cmap="inferno", s=3, alpha=0.5, linewidths=0)
    ax.scatter(*best_2d, c="cyan", s=150, marker="*", zorder=5, label="best known")
    plt.colorbar(sc, ax=ax, label="KL+IC fitness")
    ax.set_title(f"PCA PC1={var[0]*100:.1f}% PC2={var[1]*100:.1f}%")
    ax.set_xlabel("PC1"); ax.set_ylabel("PC2")
    ax.legend(fontsize=8)

    ax2 = axes[1]
    ax2.scatter(distances, sa, s=3, alpha=0.3, color="steelblue", linewidths=0)
    m, b = np.polyfit(distances, sa, 1)
    xs = np.linspace(distances.min(), distances.max(), 200)
    ax2.plot(xs, m*xs+b, color="tomato", lw=1.5, label=f"Pearson r={pearson_r:.4f}")
    ax2.scatter(0, fitness(BEST_KW1, BEST_KW2), c="cyan", s=100, marker="*", zorder=5, label="best known")
    ax2.set_xlabel("Hamming distance from best")
    ax2.set_ylabel("KL & IC fitness")
    ax2.set_title("FDC")
    ax2.legend(fontsize=8)

    plt.tight_layout()
    plt.savefig("landscape_pca_fdc.png", dpi=150, bbox_inches="tight")
    print("Saved: landscape_pca_fdc.png")
    plt.show()


# Executes independent random walks, randomly perturbing the key and registering the fitness value
# Computes autocorrelation of the fitness sequence up to RW_MAX_LAG
def autocorrelation(x: np.ndarray, max_lag) -> np.ndarray:
    x = x - x.mean()
    var = np.var(x)
    
    if var == 0:
        return np.zeros(max_lag + 1)
    
    result = [1.0]

    for lag in range(1, max_lag + 1):
        c = np.mean(x[:len(x)-lag] * x[lag:])
        result.append(c / var)

    return np.array(result)


def plot_autocorrelation():
    print("Random walk autocorrelation...")
    all_ac = []

    for _ in range(RW_N_WALKS):
        kw1, kw2 = random_key()
        walk_scores = [fitness(kw1, kw2)]

        for _ in range(RW_STEPS - 1):
            kw1, kw2 = perturb(kw1, kw2, ANALYZE)
            walk_scores.append(fitness(kw1, kw2))

        all_ac.append(autocorrelation(np.array(walk_scores), RW_MAX_LAG))

    all_ac = np.array(all_ac)
    mean_ac = all_ac.mean(axis=0)
    std_ac = all_ac.std(axis=0)
    lags = np.arange(RW_MAX_LAG + 1)

    fig, axes = plt.subplots(1, 2, figsize=(14, 5))
    fig.suptitle(f"Random Walk Autocorrelation | ANALYZE={ANALYZE} walks={RW_N_WALKS} steps={RW_STEPS}", fontsize=11)

    ax = axes[0]
    ax.plot(lags, mean_ac, color="steelblue", lw=1.5, label="mean AC")
    ax.fill_between(lags, mean_ac - std_ac, mean_ac + std_ac, alpha=0.3, color="steelblue", label="±1 std")
    ax.axhline(0, color="gray", lw=0.8, linestyle="--")
    ax.axhline(1/math.e, color="tomato", lw=0.8, linestyle=":", label=f"1/e approx. {1/math.e:.2f}")
    corr_len = next((l for l, v in enumerate(mean_ac) if v < 1/math.e), RW_MAX_LAG)
    ax.axvline(corr_len, color="green", lw=1, linestyle="--", label=f"corr. length approx. {corr_len}")
    ax.set_xlabel("Lag"); ax.set_ylabel("Autocorrelation")
    ax.set_title("Mean autocorrelation across walks")
    ax.legend(fontsize=8)

    ax2 = axes[1]
    kw1, kw2 = random_key()
    sample_walk = [fitness(kw1, kw2)]

    for _ in range(min(500, RW_STEPS) - 1):
        kw1, kw2 = perturb(kw1, kw2, ANALYZE)
        sample_walk.append(fitness(kw1, kw2))

    ax2.plot(sample_walk, color="steelblue", lw=0.8, alpha=0.8)
    ax2.set_xlabel("Step"); ax2.set_ylabel("KL+IC fitness")
    ax2.set_title("Sample random walk (first 500 steps)")

    print(f"Correlation length: approx. {corr_len} steps")
    plt.tight_layout()
    plt.savefig("landscape_autocorr.png", dpi=150, bbox_inches="tight")
    print("Saved: landscape_autocorr.png")
    plt.show()


# Discretizes fitness in MI_BINS uniform bins
# Computes Mutual Information between each key position and the fitness value
# Total MI is computed by summing (thus assuming independence between the characters of the key)
def plot_mutual_information(keys: list[tuple[str, str]], scores: list[float]):
    print("Mutual information analysis...")
    sa = np.array(scores)
    score_bins = np.digitize(sa, np.linspace(sa.min(), sa.max(), MI_BINS + 1)[:-1])

    positions = []
    mi_vals = []

    for pos in range(KW1_LENGTH):
        col = np.array([ALPHABET.index(k1[pos]) for k1, k2 in keys])
        positions.append(f"kw1[{pos}]")
        mi_vals.append(mutual_info_score(col, score_bins))

    for pos in range(KW2_LENGTH):
        col = np.array([ALPHABET.index(k2[pos]) for k1, k2 in keys])
        positions.append(f"kw2[{pos}]")
        mi_vals.append(mutual_info_score(col, score_bins))

    # Total MI: hashing the key + mapping in MI_BINS^2 bins
    mi_total = sum(mi_vals)

    fig, axes = plt.subplots(1, 2, figsize=(14, 5))
    fig.suptitle(f"Mutual Information | ANALYZE={ANALYZE} bins={MI_BINS}", fontsize=11)

    ax = axes[0]
    colors = ["steelblue"] * KW1_LENGTH + ["tomato"] * KW2_LENGTH
    bars = ax.bar(positions, mi_vals, color=colors, alpha=0.8, edgecolor="white")
    ax.set_xlabel("Key position")
    ax.set_ylabel("Mutual Information (nats)")
    ax.set_title("MI between each key position and KL+IC fitness")
    ax.legend(handles=[
        plt.Rectangle((0,0),1,1, fc="steelblue", alpha=0.8, label="kw1"),
        plt.Rectangle((0,0),1,1, fc="tomato", alpha=0.8, label="kw2")
    ], fontsize=8)
    
    for bar, val in zip(bars, mi_vals):
        ax.text(bar.get_x() + bar.get_width()/2, bar.get_height() + 0.0001, f"{val:.5f}", ha="center", va="bottom", fontsize=7)

    ax2 = axes[1]
    pearson_vals = []

    for pos in range(KW1_LENGTH):
        col = np.array([ALPHABET.index(k1[pos]) for k1, k2 in keys], float)
        pearson_vals.append(abs(np.corrcoef(col, sa)[0, 1]))

    for pos in range(KW2_LENGTH):
        col = np.array([ALPHABET.index(k2[pos]) for k1, k2 in keys], float)
        pearson_vals.append(abs(np.corrcoef(col, sa)[0, 1]))

    x = np.arange(len(positions))
    w = 0.35
    ax2.bar(x - w/2, pearson_vals, w, label="|Pearson r|", color="steelblue", alpha=0.8)
    ax2.bar(x + w/2, mi_vals, w, label="MI", color="tomato", alpha=0.8)
    ax2.set_xticks(x); ax2.set_xticklabels(positions, fontsize=8)
    ax2.set_ylabel("Value")
    ax2.set_title("|Pearson r| vs MI per position")
    ax2.legend(fontsize=8)

    print(f"Total key <--> fitness MI: {mi_total:.6f}")
    plt.tight_layout()
    plt.savefig("landscape_mi.png", dpi=150, bbox_inches="tight")
    print("Saved: landscape_mi.png")
    plt.show()


# Samples DIFF_SAMPLES random keys, computes fitness
# For each character of kw1 and kw2 swaps that characters out for a random one and computes the difference in fitness
# Accumulates these differences and shows them in box plots
def plot_fitness_differential():
    print("Fitness differential analysis...")

    delta_kw1 = [[] for _ in range(KW1_LENGTH)]
    delta_kw2 = [[] for _ in range(KW2_LENGTH)]

    for _ in range(DIFF_SAMPLES):
        kw1, kw2 = random_key()
        base = fitness(kw1, kw2)

        for pos in range(KW1_LENGTH):
            new_ch = random.choice(ALPHABET_LIST)
            new_kw1 = kw1[:pos] + new_ch + kw1[pos+1:]
            delta_kw1[pos].append(abs(fitness(new_kw1, kw2) - base))

        for pos in range(KW2_LENGTH):
            new_ch = random.choice(ALPHABET_LIST)
            new_kw2 = kw2[:pos] + new_ch + kw2[pos+1:]
            delta_kw2[pos].append(abs(fitness(kw1, new_kw2) - base))

    fig, axes = plt.subplots(1, 2, figsize=(14, 5))
    fig.suptitle(f"Fitness Differential (|Delta fitness| per single symbol change) | samples={DIFF_SAMPLES}", fontsize=11)

    axes[0].boxplot(delta_kw1, labels=[f"kw1[{i}]" for i in range(KW1_LENGTH)], patch_artist=True, showfliers=False, medianprops=dict(color="tomato", lw=2), boxprops=dict(facecolor="steelblue", alpha=0.6))
    axes[0].set_ylabel("|Delta KL+IC fitness|")
    axes[0].set_title("Sensitivity per kw1 position")

    axes[1].boxplot(delta_kw2, labels=[f"kw2[{i}]" for i in range(KW2_LENGTH)], patch_artist=True, showfliers=False, medianprops=dict(color="tomato", lw=2), boxprops=dict(facecolor="salmon", alpha=0.6))
    axes[1].set_ylabel("|Delta KL+IC fitness|")
    axes[1].set_title("Sensitivity per kw2 position")

    print("Median |Delta fitness| per position:")

    for pos, d in enumerate(delta_kw1):
        print(f"kw1[{pos}]: {np.median(d):.6f}")

    for pos, d in enumerate(delta_kw2):
        print(f"kw2[{pos}]: {np.median(d):.6f}")

    plt.tight_layout()
    plt.savefig("landscape_differential.png", dpi=150, bbox_inches="tight")
    print("Saved: landscape_differential.png")
    plt.show()


# Compares the evolution of cumulative char-by-char fitness between the correct key and N_PAIRS random keys
# Plots both the absolute curves and the separation between the best fitness and the random-key ones
def plot_round_correlation():
    print("Round-by-round correlation...")

    N_PAIRS = 10

    fig, axes = plt.subplots(1, 2, figsize=(14, 5))
    fig.suptitle(f"Round-by-round cumulative KL+IC fitness | ANALYZE={ANALYZE}", fontsize=11)

    ax = axes[0]
    ax2 = axes[1]

    best_scores = hutton_decrypt_round_scores(CIPHERTEXT, BEST_KW1, BEST_KW2)
    ax.plot(best_scores, color="cyan", lw=1.5, zorder=5, label="best known")

    separations = []

    for _ in range(N_PAIRS):
        kw1, kw2 = random_key()
        rand_scores = hutton_decrypt_round_scores(CIPHERTEXT, kw1, kw2)
        ax.plot(rand_scores, color="steelblue", lw=0.6, alpha=0.4)
        min_len = min(len(best_scores), len(rand_scores))
        diff = np.array(best_scores[:min_len]) - np.array(rand_scores[:min_len])
        ax2.plot(diff, color="tomato", lw=0.6, alpha=0.4)
        separations.append(diff)

    mean_sep = np.mean(separations, axis=0)
    ax2.plot(mean_sep, color="darkred", lw=1.5, label="mean separation")
    ax2.axhline(0, color="gray", lw=0.8, linestyle="--")

    ax.set_xlabel("Character index"); ax.set_ylabel("Cumulative KL+IC fitness")
    ax.set_title("Best vs random keys - fitness evolution")
    ax.legend(fontsize=8)

    ax2.set_xlabel("Character index")
    ax2.set_ylabel("fitness(best) - fitness(random)")
    ax2.set_title("Separation between best and random keys")
    ax2.legend(fontsize=8)

    threshold_idx = next(
        (i for i, v in enumerate(mean_sep) if v > 0 and all(mean_sep[i:i+10] > 0)),
        None
    )
    if threshold_idx is not None:
        ax2.axvline(threshold_idx, color="green", lw=1, linestyle="--", label=f"signal emerges @ char {threshold_idx}")
        ax2.legend(fontsize=8)
        print(f"Signal emerges consistently after char. {threshold_idx}")
    else:
        print("Signal never consistently emerges - landscape flat throughout")

    plt.tight_layout()
    plt.savefig("landscape_rounds.png", dpi=150, bbox_inches="tight")
    print("Saved: landscape_rounds.png")
    plt.show()


if __name__ == "__main__":
    load_tetragrams("english_quadgrams.txt")

    print(f"Configuration: KEYS={ANALYZE}, kw1={BEST_KW1}, kw2={BEST_KW2}\n")

    print(f"Sampling {N_SAMPLES} keys...")
    keys = sample_keys(N_SAMPLES)
    keys.append((BEST_KW1, BEST_KW2))
    print("Evaluating fitness...")
    scores = [fitness(k1, k2) for k1, k2 in keys]
    print(f"Score range: [{min(scores):.4f}, {max(scores):.4f}]\n")

    print("--- 1/5 PCA + FDC ---")
    plot_pca_fdc(keys, scores)

    print("\n--- 2/5 RANDOM WALK AUTOCORRELATION ---")
    plot_autocorrelation()

    print("\n--- 3/5 MUTUAL INFORMATION ---")
    plot_mutual_information(keys, scores)

    print("\n--- 4/5 FITNESS DIFFERENTIAL ---")
    plot_fitness_differential()

    print("\n--- 5/5 ROUND-BY-ROUND CORRELATION ---")
    plot_round_correlation()

    print("\nDone.")
