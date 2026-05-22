//Compile with: gcc -O3 -o hutton_topk hutton_topk.c -lm
//Use with: hutton_topk quadgrams.txt [--top-k K] [--probe N]

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <time.h>
#include <ctype.h>
#include <stdint.h>

static const char CIPHERTEXT[] = "WZABIICBDINRRXXUGFNEICNDKJWFUDRHJFSMMKDALZHGBAVSQVLVCLMUEUFWLHYPPCYXUWENBONRDHNPOFEQLUWCCKGXZSYKSXDERUCVXIQREKLDXEHZABHONXLPVIJTVQCKJKGDZMKGUPEDGAKOMERPIRWKGNGCFEROWPAOZKCIWCAVPQPKUDDIIUVXLFLFTHKQXQGKLRXPZKTOMSVZPQIJEVQORZEBJIEBCFWGRODAJUQCUCTOTZYMQQPILDIHMMDXCRTWWEOMEGGVHCLWFVXYOCVBMABNUFRNHZZYGUWMMKERYMNSEBNAQBQPIAMSZBXVBIOOUFGPXLXXOUIREQGRPWJTJZGZAXYXEQQXOAZWOXHXSNOHTFUUETKLSIEPPWFVBMMYNJSMKFOGRGMWBYKACKZRZYJEJSCOQDRPALVFTTNUFOTHNDLFPZKVSVOHGKVRJLLNZOYEAFRPRAABQRPJOELPVRTLDBJJDRQAROVHDZHAEQPVXLZEZWFSCYQPTLEOYWCTSAZFXVWEQPMZAZWFPQSLKVJAMTENZRIOJNTMEYJYRWBDVKLXKDJBZSPVMZCDSSLWPVZQRUOQVYNJLVCRCGXWABFXHCJUKWZMTDRGNUEYBZUXRBZTEVQOFYVUKOXUJMGQDECMTZGDMZFLIRLSCTAJLOFOGOQCBBCAPKJWQWWQBWKVGGFSVVODIHCHFQLGJRCHPDKJMNBFZUNLHHLMILJEXYRHRGBGACHXWJIWFFXDKQGZNFONYJDUPYYHJSAEBDMJITEKFFYJEGKXSQPDOLJUKJYPNASNNRDTYJWXCHHWQPJWUWSRJLYPNKFBLBPOIZSJKCFBHHXUVPFNADCSWKCQXFVGRGZKMPUCYFRMLNOLEAPEHVOCFNCKGDOQTHUHWCWKWLQFHRPYNWSUHOKRTMRJQWMKNLMCTVFURWKQEMETYQJIWGGBOZCZLCWWFHFVWPIKPHRWKEGANMCTWGEKFLTMOBSPZIDVAIKSXSIZFKOCGNROPHNDZLGCXWGFLEQXTPJAAFMKNDVJMAZKQUHCUCKFONBTDJIQNHNBLVMKATNKSURDKSNUKZGXNBGFTGPADWPXRGCDPHCUJINZZGSYTBWRKKXCRAEFCPKUZWPRVEAKVZOPVEYZYEGQJSPVFHQVMPCYGIPVUPXMRMSUPKJNVWKYIKUQBJVFNBYIYSBBFFSIWOJROZWKSKTUUGKSNRBYBQJYIITSIWCKXBGPCMTRKTAGWQKWTIATQXQBFGVFBFBDGBWAVYAZTBHTLSBOMFSZBEFEEWQSSBFOIVYTGASFAUEHWOAPAALZDCDFOJWKDJREZEWWSFATTCGIVAHGIDGTEMHSCZZMARPVZKJNWKNFAVYRVQIMKLDKSZRQTZTODQTSEWLDJKUOPYJOUDVRIBCPBWYBIKJUOKUFAUUGLFXFYODUVVSACYUOWHUBXYXAZFDILUMCRMZSEYPGDHKQTUYPGJNNBYJUITQLGEKZFKVTAEHHAUMNPWWEKJQJZHULYWWREZKEYGXHWSQCJRZERDWGRIASJXCBEUONDVYTZWKRPZKIDIZFZLHRNCVOGSCTBOHIZOKIWRYNHCVIBCCOIFXOBVMIDWNHUWEJZCTUHFGHMXRWEYIFBABGJDLCCLGLUGCJZZSUZOOPDBJYAGXOGDEGBGBYYVROWTQTSZNPULXHJLZKSSFTWVLGJXFHZZTOIAODVIWGXLNEDKVVQPJGGWEYARANOBLWGRYSJGKVVPMEUACBMFHWIELJHTPKWEENBSWADBVTNSDSXNHVEHLMRRAPYEDEESKNOKSBFDGPPSLGOLJLTYGFIOADETIFHRHFATVNZOCXFFDHTKJFFCLDIXHSIJJLWQDWHHARVZWRZUXVVGXYKPGIKXGHLZOZYVPFPLPDBMVKYPZOUEUSISFXUCNDLDHONUDYQIKXYZFXPWFREUDEMSKOOWEETRVPMDEEJXKENJCENZXPPTEMSEHUYKQUDIIUYGSHZCQXGXVPZYJUISYQXWPLAOAJGLXZKQNJQRDFFCBPVUWRZBJJKFTKBWZHXFMPWSRCJGURXAMKVEWLBNQXRMHPSZCDFAJBGDNPRJARGFKYDUCJAKXXCDRODYKSGDEKXNFHAELPAZRVWCQXZMMBVHVZPJZQARWTWAMLAALAERIQTPGLOWEKVCCVXEZNWPABURNTEMCJZGUPZVQSCCRSVGZVGOHULWFUFXFGXDYNSDTZLBLLZSLEWSLFDWRGYBWQOKAGCDDNMCAYQXJQOMEJWVEHKKAIDLUTXNRJJXPXUVZQNVIDLGWQKCKDUNKSZPTVZSUCHMSSFUXJTKVOCETYLZYDVLTPQWBXOVSJDXMAZNMUBGOBBFQQJPECYLWNJISULUZZJIKYHOUGCXHQGMDDXOYDCMZKUNCPBGZXLNDUETJGBBLXCKQVFOGFZXVDGEJIJFEGWXUBIPPZQKHUXFYBYZOCSSXNOYQQRDDSNGVWOUKAVIGWSVWIJVQWMFCDXLSHERGVOQADQZOQPTUGRYSBUJVKZGCURLYQINHWPPRDGSDGZGKKEPBMJTZYEZSHYFESXDTBEDHILTLEESLYNFNIKGFLZVYXSOVOKUPCSROGJBNOQQTRRACQPJGDJDHXZDSYFDXYJSKHBHXHRTCRIFXWYKEYLHDSTPFVKMMOHHDSTZDQOVNUXBEHPYXMTRMHWAETVMEPBVRNBKKTZTCNXNPQKOTYEAMKBLYDSNJNCZUVDMXPTTEBHHMKDAWZSLBTYUQSIVTPTGLIDMNYVFSBIPPKDXHOVUXFGORUXNPNBZSECSPQJYYTUXBWTZOJRRZNANKTREWMWFVHOETJJTHVMNDOCKDZTHKCGOHLPZRMQONYZDHFMSZXDSSRXIAIACPBHXZQZMWBEGQAGFGHKSXJA";

//Cipher parameters
#define KW1_LENGTH 4
#define KW2_LENGTH 5
#define ALPHA_LENGTH 26
#define ALPHABET "ABCDEFGHIJKLMNOPQRSTUVWXYZ"

//ABC Parameters
#define N_EMPLOYED 1000
#define N_ONLOOKER 500
#define LIMIT 50
#define N_CYCLES 500

//Top-K Parameters can be overwritten via CLI
static int TOP_K = 15;
static int N_PROBE = 1000000;

//Open addressing hashmap for tetragrams
#define MAP_SIZE (1 << 22)
#define MAP_MASK (MAP_SIZE - 1)

typedef struct {
    char key[5];
    float val;
} Entry;

static Entry gram_map[MAP_SIZE];
static float floor_score;

//FNV-1a Hashing for hashmap using standard constants
static unsigned fnv1a(const char *s, int n)
{
    unsigned h = 2166136261u;

    for (int i = 0; i < n; i++)
        h = (h ^ (unsigned char)s[i]) * 16777619u;

    return h;
}

//Inserts a tetragram in the hashmap
static void map_insert(const char key[4], float val)
{
    unsigned h = fnv1a(key, 4) & MAP_MASK;

    while (gram_map[h].key[0]) 
        h = (h + 1) & MAP_MASK;

    memcpy(gram_map[h].key, key, 4);

    gram_map[h].key[4] = '\0';
    gram_map[h].val = val;
}

//Lookup the score of a tetragram in the hashmap
static float map_get(const char key[4])
{
    unsigned h = fnv1a(key, 4) & MAP_MASK;

    while (gram_map[h].key[0]){
        if (memcmp(gram_map[h].key, key, 4) == 0)
            return gram_map[h].val;

        h = (h + 1) & MAP_MASK;
    }

    return floor_score;
}

//Reads the quadgrams file row by row and accumulates score in the Raw array
//Inserts log values of the normalized scores in the hashmap
static void load_tetragrams(const char *filename)
{
    printf("Loading tetragrams...\n");

    FILE *f = fopen(filename, "r");
    if (!f){
        perror(filename); 
        exit(1);
    }

    char gram[16];
    long long count;
    long long total = 0;

    typedef struct {
        char g[5];
        long long c;
    } Raw;

    Raw *raw = NULL;
    int nraw = 0;
    int cap = 0;

    while (fscanf(f, "%4s %lld", gram, &count) == 2){
        if (nraw == cap){
            cap = cap ? cap * 2 : 65536;
            raw = realloc(raw, cap * sizeof(Raw));
        }
        memcpy(raw[nraw].g, gram, 4);
        raw[nraw].g[4] = '\0';
        raw[nraw].c = count;
        nraw++;
        total += count;
    }
    fclose(f);

    floor_score = (float)log10(0.01 / (double)total);

    for (int i = 0; i < nraw; i++)
        map_insert(raw[i].g, (float)log10((double)raw[i].c / (double)total));

    free(raw);

    printf("%d tetragrams loaded\n\n", nraw);
}

//xoshiro256** PRNG state
static uint64_t rng_s[4];

//Bitwise rotation to the left
static inline uint64_t rotl64(uint64_t x, int k)
{
    return (x << k) | (x >> (64 - k));
}

//Implementation of xoshiro256**
static uint64_t rng_next(void)
{
    uint64_t r = rotl64(rng_s[1] * 5, 7) * 9;
    uint64_t t = rng_s[1] << 17;
    rng_s[2] ^= rng_s[0]; rng_s[3] ^= rng_s[1];
    rng_s[1] ^= rng_s[2]; rng_s[0] ^= rng_s[3];
    rng_s[2] ^= t; rng_s[3] = rotl64(rng_s[3], 45);

    return r;
}

//Converts rng_next() output in a double
static inline double rng_double(void)
{
    return (rng_next() >> 11) * (1.0 / (1ULL << 53));
}

//Converts rng_double() in an integer by truncation
static inline int rng_int(int n)
{
    return (int)(rng_double() * n);
}

//Converts rng_int() output in a character over the alphabet
static inline char rng_alpha(void)
{
    return (char)('A' + rng_int(ALPHA_LENGTH));
}

//Hutton cipher helpers

static int CT_LEN;

//Build the initial cipher alphabet from kw2
static void make_cipher_alphabet(const char *kw2, char ca[ALPHA_LENGTH])
{
    int seen[ALPHA_LENGTH] = {0};
    int idx = 0;

    for (int i = 0; i < KW2_LENGTH; i++){
        int c = kw2[i] - 'A';

        if (!seen[c]){
            ca[idx++] = kw2[i];
            seen[c] = 1;
        }
    }

    for (int c = 0; c < ALPHA_LENGTH; c++)
        if (!seen[c])
            ca[idx++] = (char)('A' + c);
}

//Given kw1 and kw2, attempt to decrypt the ciphertext
static void hutton_decrypt(const char *kw1, const char *kw2, char *pt)
{
    char ca[ALPHA_LENGTH];
    make_cipher_alphabet(kw2, ca);
    int pos[ALPHA_LENGTH];

    for (int i = 0; i < ALPHA_LENGTH; i++)
        pos[(int)(ca[i]-'A')] = i;

    for (int i = 0; i < CT_LEN; i++){
        int shift = (kw1[i % KW1_LENGTH] - 'A') + 1;
        int idx_swap = pos[(int)(CIPHERTEXT[i] - 'A')];
        int idx_pt = ((idx_swap - shift) % ALPHA_LENGTH + ALPHA_LENGTH) % ALPHA_LENGTH;

        pt[i] = ca[idx_pt];
        pos[(int)(ca[idx_pt] - 'A')] = idx_swap;
        pos[(int)(ca[idx_swap]- 'A')] = idx_pt;
        char tmp = ca[idx_pt];
        ca[idx_pt] = ca[idx_swap];
        ca[idx_swap] = tmp;
    }

    pt[CT_LEN] = '\0';
}

//Compute fitness
static float fitness(const char *kw1, const char *kw2)
{
    char pt[10000];
    hutton_decrypt(kw1, kw2, pt);
    float sum = 0.0f;

    for (int i = 0; i <= CT_LEN - 4; i++)
        sum += map_get(pt + i);

    return sum / (float)(CT_LEN - 3);
}

//For each position p of kw1 and for each letter l of the alphabet compute the mean of the fitness over samples in which kw1[p] = l
//Samples a total of N_PROBE random keys kw1+kw2
static void estimate_letter_fitness(double mean_fit[KW1_LENGTH][ALPHA_LENGTH])
{
    printf("Probing %d random keys to estimate per-position fitness...\n", N_PROBE);

    double sum[KW1_LENGTH][ALPHA_LENGTH];
    int cnt[KW1_LENGTH][ALPHA_LENGTH];
    memset(sum, 0, sizeof(sum));
    memset(cnt, 0, sizeof(cnt));

    char kw1[KW1_LENGTH + 1], kw2[KW2_LENGTH + 1];
    kw1[KW1_LENGTH] = '\0';
    kw2[KW2_LENGTH] = '\0';

    for (int s = 0; s < N_PROBE; s++){
        for (int i = 0; i < KW1_LENGTH; i++)
            kw1[i] = rng_alpha();

        for (int i = 0; i < KW2_LENGTH; i++)
            kw2[i] = rng_alpha();

        float f = fitness(kw1, kw2);

        for (int p = 0; p < KW1_LENGTH; p++){
            int c = kw1[p] - 'A';
            sum[p][c] += f;
            cnt[p][c]++;
        }
    }

    for (int p = 0; p < KW1_LENGTH; p++)
        for (int c = 0; c < ALPHA_LENGTH; c++)
            mean_fit[p][c] = (cnt[p][c] > 0) ? sum[p][c] / cnt[p][c] : -1e9;

    printf("Done.\n\n");
}


//Selects TOP_K candidates for each position of kw1 based on mean_fit
static void select_top_k(const double mean_fit[KW1_LENGTH][ALPHA_LENGTH], int candidates[KW1_LENGTH][ALPHA_LENGTH])
{
    printf("Top-%d candidates per kw1 position:\n", TOP_K);

    for (int p = 0; p < KW1_LENGTH; p++){
        int order[ALPHA_LENGTH];

        for (int c = 0; c < ALPHA_LENGTH; c++)
            order[c] = c;

        for (int k = 0; k < TOP_K; k++){
            int best_j = k;

            for (int j = k + 1; j < ALPHA_LENGTH; j++)
                if (mean_fit[p][order[j]] > mean_fit[p][order[best_j]])
                    best_j = j;

            int tmp = order[k];
            order[k] = order[best_j];
            order[best_j] = tmp;
            candidates[p][k] = order[k];
        }

        printf("Pos. %d: ", p);

        for (int k = 0; k < TOP_K; k++)
            printf("%c(%.5f) ", 'A' + candidates[p][k], mean_fit[p][candidates[p][k]]);

        printf("\n");
    }
    printf("\n");
}

//Food source for bees in ABC
typedef struct {
    char kw2[KW2_LENGTH + 1];
    float score;
    int trials;
} Source;

static Source abc_sources[N_EMPLOYED];
static double abc_probs[N_EMPLOYED];

//Initializes N_EMPLOYED food sources with fixed kw1 and random kw2
static void abc_init(const char *fixed_kw1)
{
    char kw2[KW2_LENGTH + 1];
    kw2[KW2_LENGTH] = '\0';

    for (int i = 0; i < N_EMPLOYED; i++){
        for (int j = 0; j < KW2_LENGTH; j++)
            kw2[j] = rng_alpha();

        memcpy(abc_sources[i].kw2, kw2, KW2_LENGTH + 1);
        abc_sources[i].score = fitness(fixed_kw1, kw2);
        abc_sources[i].trials = 0;
    }
}

//Generates two neighbors of the food source: one by substitution and one by swapping. The best one gets picked
static void abc_neighbour(const char *fixed_kw1, const Source *src, Source *dst)
{
    *dst = *src;
    
    //Candidate A: substitution
    Source ca = *src;
    {
        int pos = rng_int(KW2_LENGTH);
        ca.kw2[pos] = rng_alpha();
    }

    ca.score = fitness(fixed_kw1, ca.kw2);

    //Candidate B: swap of two letters
    Source cb = *src;
    {
        int p = rng_int(KW2_LENGTH);
        int q = rng_int(KW2_LENGTH - 1);

        if (q >= p)
            q++;

        char tmp = cb.kw2[p];
        cb.kw2[p] = cb.kw2[q];
        cb.kw2[q] = tmp;
    }
    
    cb.score = fitness(fixed_kw1, cb.kw2);

    *dst = (ca.score >= cb.score) ? ca : cb;
}

//Computes selection probabilities for onlooker bees by normalizing weights exp(score - max_score)
static void abc_compute_probs(void)
{
    double max_s = abc_sources[0].score;

    for (int i = 1; i < N_EMPLOYED; i++)
        if (abc_sources[i].score > max_s)
            max_s = abc_sources[i].score;

    double tot = 0.0;

    for (int i = 0; i < N_EMPLOYED; i++){
        abc_probs[i] = exp(abc_sources[i].score - max_s);
        tot += abc_probs[i];
    }

    for (int i = 0; i < N_EMPLOYED; i++)
        abc_probs[i] /= tot;
}

//Samples an index from abc_probs performing roulette wheel selection
static int abc_roulette(void)
{
    double r = rng_double(), cumul = 0.0;

    for (int i = 0; i < N_EMPLOYED; i++){
        cumul += abc_probs[i];

        if (r <= cumul)
            return i;
    }

    return N_EMPLOYED - 1;
}

//ABC main loop
static float abc_run(const char *fixed_kw1, char best_kw2_out[KW2_LENGTH + 1])
{
    //Fixes kw1
    abc_init(fixed_kw1);

    int bi = 0;
    for (int i = 1; i < N_EMPLOYED; i++)
        if (abc_sources[i].score > abc_sources[bi].score)
            bi = i;

    float best_score = abc_sources[bi].score;
    memcpy(best_kw2_out, abc_sources[bi].kw2, KW2_LENGTH + 1);

    Source nb;
    for (int cycle = 0; cycle < N_CYCLES; cycle++){

        //Employed phase: employed bees generate neightbors
        for (int i = 0; i < N_EMPLOYED; i++){
            abc_neighbour(fixed_kw1, &abc_sources[i], &nb);

            if (nb.score > abc_sources[i].score){
                abc_sources[i] = nb;
                abc_sources[i].trials = 0;
            } 
            else{
                //If the neighbor is worse, increment failed trials
                abc_sources[i].trials++;
            }
        }

        abc_compute_probs();

        //Onlooker phase: using the same logic, onlooker bees explore the neighborhood of solution selected via roulette wheel selection
        for (int b = 0; b < N_ONLOOKER; b++){
            int idx = abc_roulette();
            abc_neighbour(fixed_kw1, &abc_sources[idx], &nb);

            if (nb.score > abc_sources[idx].score){
                abc_sources[idx] = nb;
                abc_sources[idx].trials = 0;
            }
            else{
                abc_sources[idx].trials++;
            }
        }
        
        //Scout phase: if trials limit has been exceeded the corresponding sources are replaced with random kw2
        for (int i = 0; i < N_EMPLOYED; i++){
            if (abc_sources[i].trials >= LIMIT){
                char kw2[KW2_LENGTH + 1];
                kw2[KW2_LENGTH] = '\0';

                for (int j = 0; j < KW2_LENGTH; j++)
                    kw2[j] = rng_alpha();

                memcpy(abc_sources[i].kw2, kw2, KW2_LENGTH + 1);
                abc_sources[i].score = fitness(fixed_kw1, kw2);
                abc_sources[i].trials = 0;
            }
        }
        int cb = 0;
        for (int i = 1; i < N_EMPLOYED; i++)
            if (abc_sources[i].score > abc_sources[cb].score)
                cb = i;

        //Tracks the global best score
        if (abc_sources[cb].score > best_score){
            best_score = abc_sources[cb].score;
            memcpy(best_kw2_out, abc_sources[cb].kw2, KW2_LENGTH + 1);
        }
    }

    return best_score;
}

//Brute-forces each of the TOP_K^KW1_LENGTH candidates for kw1 and runs ABC on each fixed kw1
static void brute_force(const int candidates[KW1_LENGTH][ALPHA_LENGTH])
{
    long total_kw1 = 1;

    for (int p = 0; p < KW1_LENGTH; p++)
        total_kw1 *= TOP_K;

    printf("Brute-forcing %ld kw1 candidates...\n\n", total_kw1);

    float best_score = -1e30f;
    char best_kw1[KW1_LENGTH + 1];
    char best_kw2[KW2_LENGTH + 1];
    char best_pt[10000];

    best_kw1[KW1_LENGTH] = '\0';
    best_kw2[KW2_LENGTH] = '\0';

    int idx[KW1_LENGTH];
    memset(idx, 0, sizeof(idx));

    for (long combo = 0; combo < total_kw1; combo++){

        //Build kw1
        char kw1[KW1_LENGTH + 1];
        kw1[KW1_LENGTH] = '\0';

        for (int p = 0; p < KW1_LENGTH; p++)
            kw1[p] = (char)('A' + candidates[p][idx[p]]);

        //Run ABC with fixed kw1
        char kw2[KW2_LENGTH + 1];
        float score = abc_run(kw1, kw2);

        char pt[10000];
        hutton_decrypt(kw1, kw2, pt);

        printf("[%ld/%ld] kw1=%s, kw2=%s, score=%.6f, pt=%.60s", combo+1, total_kw1, kw1, kw2, (double)score, pt);

        if (score > best_score){
            best_score = score;
            memcpy(best_kw1, kw1, KW1_LENGTH + 1);
            memcpy(best_kw2, kw2, KW2_LENGTH + 1);
            memcpy(best_pt, pt, CT_LEN + 1);

            printf(" - new best!");
        }
        printf("\n");

        for (int p = KW1_LENGTH - 1; p >= 0; p--){
            if (++idx[p] < TOP_K)
                break;

            idx[p] = 0;
        }
    }

    printf("\n-------------------------------------------------------------------\n");
    printf("OVERALL BEST\n");
    printf("kw1: %s\n", best_kw1);
    printf("kw2: %s\n", best_kw2);
    printf("Score: %.6f\n", (double)best_score);
    printf("Plaintext: %s\n", best_pt);
    printf("-------------------------------------------------------------------\n");
}

int main(int argc, char *argv[])
{
    if (argc < 2){
        fprintf(stderr, "Run as: %s quadgrams.txt [--top-k K] [--probe N]\n", argv[0]);
        return 1;
    }

    for (int i = 2; i < argc; i++){
        if (strcmp(argv[i], "--top-k") == 0 && i + 1 < argc)
            TOP_K = atoi(argv[++i]);

        else if (strcmp(argv[i], "--probe") == 0 && i + 1 < argc)
            N_PROBE = atoi(argv[++i]);

        else{
            fprintf(stderr, "Unrecognized argument: %s\n", argv[i]);
            return 1;
        }
    }

    if (TOP_K < 1 || TOP_K > ALPHA_LENGTH){
        fprintf(stderr, "top-k must be between 1 and %d\n", ALPHA_LENGTH);
        return 1;
    }

    //xoshiro256** PRNG seed
    //Also performs null calls to break initial correlation with seed constants
    uint64_t seed = (uint64_t)time(NULL) ^ (uint64_t)clock();
    rng_s[0] = seed * 0x9e3779b97f4a7c15ULL;
    rng_s[1] = seed * 0x6c62272e07bb0142ULL;
    rng_s[2] = ~seed;
    rng_s[3] = seed ^ 0xdeadbeefcafeULL;

    for (int i = 0; i < 20; i++)
        rng_next();

    CT_LEN = (int)strlen(CIPHERTEXT);
    load_tetragrams(argv[1]);

    printf("Parameters: TOP_K=%d, N_PROBE=%d\nKW1_LENGTH=%d, KW2_LENGTH=%d\nN_EMPLOYED=%d, N_ONLOOKER=%d, LIMIT=%d, N_CYCLES=%d\n\n", TOP_K, N_PROBE, KW1_LENGTH, KW2_LENGTH, N_EMPLOYED, N_ONLOOKER, LIMIT, N_CYCLES);

    //Estimation of kw1 position-based fitness
    double mean_fit[KW1_LENGTH][ALPHA_LENGTH];
    estimate_letter_fitness(mean_fit);

    //Choosing TOP_K characters for each position
    int candidates[KW1_LENGTH][ALPHA_LENGTH];
    select_top_k(mean_fit, candidates);

    //Reduced brute-force with ABC
    brute_force(candidates);

    return 0;
}
