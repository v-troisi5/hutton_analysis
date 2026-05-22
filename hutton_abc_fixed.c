//Compile with: gcc -O3 -o hutton_abc_fixed hutton_abc_fixed.c -lm
//Use with: hutton_abc_fixed quadgrams.txt
//Use with: hutton_abc_fixed quadgrams.txt --fix-kw1 HELLO
//Use with: hutton_abc_fixed quadgrams.txt --fix-kw2 HELLO

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <time.h>
#include <ctype.h>

static const char CIPHERTEXT[] = "WZABIICBDINRRXXUGFNEICNDKJWFUDRHJFSMMKDALZHGBAVSQVLVCLMUEUFWLHYPPCYXUWENBONRDHNPOFEQLUWCCKGXZSYKSXDERUCVXIQREKLDXEHZABHONXLPVIJTVQCKJKGDZMKGUPEDGAKOMERPIRWKGNGCFEROWPAOZKCIWCAVPQPKUDDIIUVXLFLFTHKQXQGKLRXPZKTOMSVZPQIJEVQORZEBJIEBCFWGRODAJUQCUCTOTZYMQQPILDIHMMDXCRTWWEOMEGGVHCLWFVXYOCVBMABNUFRNHZZYGUWMMKERYMNSEBNAQBQPIAMSZBXVBIOOUFGPXLXXOUIREQGRPWJTJZGZAXYXEQQXOAZWOXHXSNOHTFUUETKLSIEPPWFVBMMYNJSMKFOGRGMWBYKACKZRZYJEJSCOQDRPALVFTTNUFOTHNDLFPZKVSVOHGKVRJLLNZOYEAFRPRAABQRPJOELPVRTLDBJJDRQAROVHDZHAEQPVXLZEZWFSCYQPTLEOYWCTSAZFXVWEQPMZAZWFPQSLKVJAMTENZRIOJNTMEYJYRWBDVKLXKDJBZSPVMZCDSSLWPVZQRUOQVYNJLVCRCGXWABFXHCJUKWZMTDRGNUEYBZUXRBZTEVQOFYVUKOXUJMGQDECMTZGDMZFLIRLSCTAJLOFOGOQCBBCAPKJWQWWQBWKVGGFSVVODIHCHFQLGJRCHPDKJMNBFZUNLHHLMILJEXYRHRGBGACHXWJIWFFXDKQGZNFONYJDUPYYHJSAEBDMJITEKFFYJEGKXSQPDOLJUKJYPNASNNRDTYJWXCHHWQPJWUWSRJLYPNKFBLBPOIZSJKCFBHHXUVPFNADCSWKCQXFVGRGZKMPUCYFRMLNOLEAPEHVOCFNCKGDOQTHUHWCWKWLQFHRPYNWSUHOKRTMRJQWMKNLMCTVFURWKQEMETYQJIWGGBOZCZLCWWFHFVWPIKPHRWKEGANMCTWGEKFLTMOBSPZIDVAIKSXSIZFKOCGNROPHNDZLGCXWGFLEQXTPJAAFMKNDVJMAZKQUHCUCKFONBTDJIQNHNBLVMKATNKSURDKSNUKZGXNBGFTGPADWPXRGCDPHCUJINZZGSYTBWRKKXCRAEFCPKUZWPRVEAKVZOPVEYZYEGQJSPVFHQVMPCYGIPVUPXMRMSUPKJNVWKYIKUQBJVFNBYIYSBBFFSIWOJROZWKSKTUUGKSNRBYBQJYIITSIWCKXBGPCMTRKTAGWQKWTIATQXQBFGVFBFBDGBWAVYAZTBHTLSBOMFSZBEFEEWQSSBFOIVYTGASFAUEHWOAPAALZDCDFOJWKDJREZEWWSFATTCGIVAHGIDGTEMHSCZZMARPVZKJNWKNFAVYRVQIMKLDKSZRQTZTODQTSEWLDJKUOPYJOUDVRIBCPBWYBIKJUOKUFAUUGLFXFYODUVVSACYUOWHUBXYXAZFDILUMCRMZSEYPGDHKQTUYPGJNNBYJUITQLGEKZFKVTAEHHAUMNPWWEKJQJZHULYWWREZKEYGXHWSQCJRZERDWGRIASJXCBEUONDVYTZWKRPZKIDIZFZLHRNCVOGSCTBOHIZOKIWRYNHCVIBCCOIFXOBVMIDWNHUWEJZCTUHFGHMXRWEYIFBABGJDLCCLGLUGCJZZSUZOOPDBJYAGXOGDEGBGBYYVROWTQTSZNPULXHJLZKSSFTWVLGJXFHZZTOIAODVIWGXLNEDKVVQPJGGWEYARANOBLWGRYSJGKVVPMEUACBMFHWIELJHTPKWEENBSWADBVTNSDSXNHVEHLMRRAPYEDEESKNOKSBFDGPPSLGOLJLTYGFIOADETIFHRHFATVNZOCXFFDHTKJFFCLDIXHSIJJLWQDWHHARVZWRZUXVVGXYKPGIKXGHLZOZYVPFPLPDBMVKYPZOUEUSISFXUCNDLDHONUDYQIKXYZFXPWFREUDEMSKOOWEETRVPMDEEJXKENJCENZXPPTEMSEHUYKQUDIIUYGSHZCQXGXVPZYJUISYQXWPLAOAJGLXZKQNJQRDFFCBPVUWRZBJJKFTKBWZHXFMPWSRCJGURXAMKVEWLBNQXRMHPSZCDFAJBGDNPRJARGFKYDUCJAKXXCDRODYKSGDEKXNFHAELPAZRVWCQXZMMBVHVZPJZQARWTWAMLAALAERIQTPGLOWEKVCCVXEZNWPABURNTEMCJZGUPZVQSCCRSVGZVGOHULWFUFXFGXDYNSDTZLBLLZSLEWSLFDWRGYBWQOKAGCDDNMCAYQXJQOMEJWVEHKKAIDLUTXNRJJXPXUVZQNVIDLGWQKCKDUNKSZPTVZSUCHMSSFUXJTKVOCETYLZYDVLTPQWBXOVSJDXMAZNMUBGOBBFQQJPECYLWNJISULUZZJIKYHOUGCXHQGMDDXOYDCMZKUNCPBGZXLNDUETJGBBLXCKQVFOGFZXVDGEJIJFEGWXUBIPPZQKHUXFYBYZOCSSXNOYQQRDDSNGVWOUKAVIGWSVWIJVQWMFCDXLSHERGVOQADQZOQPTUGRYSBUJVKZGCURLYQINHWPPRDGSDGZGKKEPBMJTZYEZSHYFESXDTBEDHILTLEESLYNFNIKGFLZVYXSOVOKUPCSROGJBNOQQTRRACQPJGDJDHXZDSYFDXYJSKHBHXHRTCRIFXWYKEYLHDSTPFVKMMOHHDSTZDQOVNUXBEHPYXMTRMHWAETVMEPBVRNBKKTZTCNXNPQKOTYEAMKBLYDSNJNCZUVDMXPTTEBHHMKDAWZSLBTYUQSIVTPTGLIDMNYVFSBIPPKDXHOVUXFGORUXNPNBZSECSPQJYYTUXBWTZOJRRZNANKTREWMWFVHOETJJTHVMNDOCKDZTHKCGOHLPZRMQONYZDHFMSZXDSSRXIAIACPBHXZQZMWBEGQAGFGHKSXJA";

//Cipher parameters
#define KW1_LENGTH 4
#define KW2_LENGTH 5
#define ALPHABET "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
#define ALPHA_LENGTH 26

//ABC Parameters
#define N_EMPLOYED 1000
#define N_ONLOOKER 500
#define LIMIT 200
#define N_CYCLES 5000

//Fixed keys, in case of usage
static int FIXED_KW1 = 0;
static int FIXED_KW2 = 0;
static char fixed_kw1[KW1_LENGTH + 1];
static char fixed_kw2[KW2_LENGTH + 1];

//Open addressing hashmap for tetragrams
#define MAP_SIZE (1 << 22)
#define MAP_MASK (MAP_SIZE - 1)

typedef struct{
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

    typedef struct{
        char g[5];
        long long c;
    } Raw;

    Raw *raw = NULL;
    int nraw = 0;
    int cap = 0;

    while (fscanf(f, "%8s %lld", gram, &count) == 2){
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

//Random utils
static inline int rand_alpha(void)
{
    return rand() % ALPHA_LENGTH;
}

static inline int rand_range(int n)
{
    return rand() % n;
}

static inline double rand_double(void)
{
    return rand() / (RAND_MAX + 1.0);
}

//Hutton cipher helpers

static int CT_LEN;

//Build the initial cipher alphabet from kw2
static void make_cipher_alphabet(const char *kw2, int kw2_len, char ca[ALPHA_LENGTH])
{
    int seen[ALPHA_LENGTH] = {0};
    int idx = 0;

    for (int i = 0; i < kw2_len; i++){
        int c = toupper((unsigned char)kw2[i]) - 'A';

        if (c >= 0 && c < ALPHA_LENGTH && !seen[c]){
            ca[idx++] = (char)('A' + c);
            seen[c] = 1;
        }
    }

    for (int c = 0; c < ALPHA_LENGTH; c++)
        if (!seen[c])
            ca[idx++] = (char)('A' + c);
}

//Given kw1 and kw2, attempt to decrypt the ciphertext
static void hutton_decrypt_v1(const char *ct, int ct_len, const char *kw1, int kw1_len, const char *kw2, int kw2_len, char *pt)
{
    char ca[ALPHA_LENGTH];
    make_cipher_alphabet(kw2, kw2_len, ca);
    int pos[ALPHA_LENGTH];

    for (int i = 0; i < ALPHA_LENGTH; i++)
        pos[(unsigned char)ca[i] - 'A'] = i;

    for (int i = 0; i < ct_len; i++){
        int k1_ch = toupper((unsigned char)kw1[i % kw1_len]) - 'A';
        int shift = k1_ch + 1;
        int idx_swap = pos[(unsigned char)ct[i] - 'A'];
        int idx_pt = ((idx_swap - shift) % ALPHA_LENGTH + ALPHA_LENGTH) % ALPHA_LENGTH;

        pt[i] = ca[idx_pt];
        pos[(unsigned char)ca[idx_pt] - 'A'] = idx_swap;
        pos[(unsigned char)ca[idx_swap] - 'A'] = idx_pt;
        char tmp = ca[idx_pt];
        ca[idx_pt] = ca[idx_swap];
        ca[idx_swap] = tmp;
    }

    pt[ct_len] = '\0';
}

//Compute fitness
static float fitness(const char kw1[KW1_LENGTH], const char kw2[KW2_LENGTH])
{
    char pt[10000];
    hutton_decrypt_v1(CIPHERTEXT, CT_LEN, kw1, KW1_LENGTH, kw2, KW2_LENGTH, pt);

    int n = CT_LEN;
    if (n < 4)
        return floor_score;

    float sum = 0.0f;

    for (int i = 0; i <= n - 4; i++)
        sum += map_get(pt + i);

    return sum / (float)(n - 3);
}

//Food source for bees in ABC
typedef struct{
    char kw1[KW1_LENGTH + 1];
    char kw2[KW2_LENGTH + 1];
    float score;
    int trials;
} Source;

static void fs_random(Source *s)
{
    if (FIXED_KW1)
        memcpy(s->kw1, fixed_kw1, KW1_LENGTH + 1);
    else{
        for (int i = 0; i < KW1_LENGTH; i++)
            s->kw1[i] = (char)('A' + rand_alpha());

        s->kw1[KW1_LENGTH] = '\0';
    }

    if (FIXED_KW2)
        memcpy(s->kw2, fixed_kw2, KW2_LENGTH + 1);
    else{
        for (int i = 0; i < KW2_LENGTH; i++)
            s->kw2[i] = (char)('A' + rand_alpha());

        s->kw2[KW2_LENGTH] = '\0';
    }

    s->score = fitness(s->kw1, s->kw2);
    s->trials = 0;
}

//Generates two neighbors of the food source: one by substitution and one by swapping. The best one gets picked
//Fixed keys are not touched at all
static void fs_neighbour(const Source *src, Source *nb)
{
    //Determines which key is available for mutation
    int can_mut_kw1 = !FIXED_KW1;
    int can_mut_kw2 = !FIXED_KW2;

    if (!can_mut_kw1 && !can_mut_kw2){
        *nb = *src;
        return;
    }

    //Candidate A: substitution
    Source ca = *src;
   {
        int use_kw1;

        if(!can_mut_kw2)
            use_kw1 = 1;
        else if(!can_mut_kw1)
            use_kw1 = 0;
        else
            use_kw1 = (rand_double() < 0.5);

        if (use_kw1){
            int pos = rand_range(KW1_LENGTH);
            ca.kw1[pos] = (char)('A' + rand_alpha());
        }
        else{
            int pos = rand_range(KW2_LENGTH);
            ca.kw2[pos] = (char)('A' + rand_alpha());
        }
    }

    ca.score = fitness(ca.kw1, ca.kw2);
    ca.trials = 0;

    //Candidate B: swap of two letters
    Source cb = *src;
   {
        int use_kw1;

        if(!can_mut_kw2)
            use_kw1 = 1;
        else if(!can_mut_kw1)
            use_kw1 = 0;
        else
            use_kw1 = (rand_double() < 0.5);

        if(use_kw1){
            int p = rand_range(KW1_LENGTH);
            int q = rand_range(KW1_LENGTH - 1);

            if (q >= p)
                q++;

            char tmp = cb.kw1[p];
            cb.kw1[p] = cb.kw1[q];
            cb.kw1[q] = tmp;
        }
        else{
            int p = rand_range(KW2_LENGTH);
            int q = rand_range(KW2_LENGTH - 1);

            if (q >= p)
                q++;

            char tmp = cb.kw2[p];
            cb.kw2[p] = cb.kw2[q];
            cb.kw2[q] = tmp;
        }
    }

    cb.score = fitness(cb.kw1, cb.kw2);
    cb.trials = 0;

    *nb = (ca.score >= cb.score) ? ca : cb;
}


static Source sources[N_EMPLOYED];
static double probs[N_EMPLOYED];

//Computes selection probabilities for onlooker bees by normalizing weights exp(score - max_score)
static void compute_probs(void)
{
    double max_score = sources[0].score;

    for (int i = 1; i < N_EMPLOYED; i++)
        if (sources[i].score > max_score)
            max_score = sources[i].score;

    double total = 0.0;

    for (int i = 0; i < N_EMPLOYED; i++){
        probs[i] = exp(sources[i].score - max_score);
        total += probs[i];
    }

    for (int i = 0; i < N_EMPLOYED; i++)
        probs[i] /= total;
}

//Samples an index from abc_probs performing roulette wheel selection
static int roulette_select(void)
{
    double r = rand_double();
    double cumul = 0.0;

    for (int i = 0; i < N_EMPLOYED; i++){
        cumul += probs[i];

        if (r <= cumul)
            return i;
    }

    return N_EMPLOYED - 1;
}

//ABC main loop
static void run(void)
{
    for (int i = 0; i < N_EMPLOYED; i++)
        fs_random(&sources[i]);

    int best_idx = 0;
    for (int i = 1; i < N_EMPLOYED; i++)
        if (sources[i].score > sources[best_idx].score)
            best_idx = i;

    float best_score = sources[best_idx].score;
    char best_kw1[KW1_LENGTH + 1];
    char best_kw2[KW2_LENGTH + 1];
    char best_pt[10000];

    memcpy(best_kw1, sources[best_idx].kw1, KW1_LENGTH + 1);
    memcpy(best_kw2, sources[best_idx].kw2, KW2_LENGTH + 1);
    hutton_decrypt_v1(CIPHERTEXT, CT_LEN, best_kw1, KW1_LENGTH, best_kw2, KW2_LENGTH, best_pt);

    Source nb;

    for (int cycle = 1; cycle <= N_CYCLES; cycle++){

        //Employed phase: employed bees generate neightbors
        for (int i = 0; i < N_EMPLOYED; i++){
            fs_neighbour(&sources[i], &nb);

            if (nb.score > sources[i].score){
                sources[i] = nb;
                sources[i].trials = 0;
            }
            else{
                //If the neighbor is worse, increment failed trials
                sources[i].trials++;
            }
        }

        compute_probs();

        //Onlooker phase: using the same logic, onlooker bees explore the neighborhood of solution selected via roulette wheel selection
        for (int b = 0; b < N_ONLOOKER; b++){
            int idx = roulette_select();
            fs_neighbour(&sources[idx], &nb);

            if (nb.score > sources[idx].score){
                sources[idx] = nb;
                sources[idx].trials = 0;
            }
            else{
                sources[idx].trials++;
            }
        }

        //Scout phase: if trials limit has been exceeded the corresponding sources are replaced with random kw2
        for (int i = 0; i < N_EMPLOYED; i++)
            if (sources[i].trials >= LIMIT)
                fs_random(&sources[i]);

        //Tracks the global best score
        int cb_idx = 0;
        for (int i = 1; i < N_EMPLOYED; i++)
            if (sources[i].score > sources[cb_idx].score) cb_idx = i;

        const char *marker = "";
        if (sources[cb_idx].score > best_score){
            best_score = sources[cb_idx].score;
            memcpy(best_kw1, sources[cb_idx].kw1, KW1_LENGTH + 1);
            memcpy(best_kw2, sources[cb_idx].kw2, KW2_LENGTH + 1);
            hutton_decrypt_v1(CIPHERTEXT, CT_LEN, best_kw1, KW1_LENGTH, best_kw2, KW2_LENGTH, best_pt);
            marker = " - new best!";
        }

        char pt60[10000];
        hutton_decrypt_v1(CIPHERTEXT, CT_LEN, sources[cb_idx].kw1, KW1_LENGTH, sources[cb_idx].kw2, KW2_LENGTH, pt60);
        pt60[60] = '\0';

        printf("[Cycle %4d], score=%.6f, kw1=%s, kw2=%s, pt=%.60s%s\n", cycle, (double)sources[cb_idx].score, sources[cb_idx].kw1, sources[cb_idx].kw2, pt60, marker);
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
        fprintf(stderr, "Run as: %s quadgrams.txt [--fix-kw1 KW1] [--fix-kw2 KW2]\n", argv[0]);
        return 1;
    }

    for (int i = 2; i < argc; i++){
        if (strcmp(argv[i], "--fix-kw1") == 0 && i + 1 < argc){
            i++;
            int len = (int)strlen(argv[i]);

            if (len != KW1_LENGTH){
                fprintf(stderr, "--fix-kw1 needs exactly %d characters (received %d)\n",KW1_LENGTH, len);
                return 1;
            }

            for (int j = 0; j < KW1_LENGTH; j++)
                fixed_kw1[j] = (char)toupper((unsigned char)argv[i][j]);
            fixed_kw1[KW1_LENGTH] = '\0';
            FIXED_KW1 = 1;
            printf("Fixed kw1: %s\n", fixed_kw1);

        }
        else if (strcmp(argv[i], "--fix-kw2") == 0 && i + 1 < argc){
            i++;
            int len = (int)strlen(argv[i]);

            if (len != KW2_LENGTH){
                fprintf(stderr, "--fix-kw2 needs exactly %d characters (received %d)\n", KW2_LENGTH, len);
                return 1;
            }

            for (int j = 0; j < KW2_LENGTH; j++)
                fixed_kw2[j] = (char)toupper((unsigned char)argv[i][j]);
            fixed_kw2[KW2_LENGTH] = '\0';
            FIXED_KW2 = 1;
            printf("Fixed kw2: %s\n", fixed_kw2);

        }
        else{
            fprintf(stderr, "Unrecognized argument: %s\n", argv[i]);
            return 1;
        }
    }

    if (FIXED_KW1 && FIXED_KW2){
        fprintf(stderr, "You fixed both keys. Nothing to optimize\n");
        return 1;
    }

    srand((unsigned)time(NULL));
    CT_LEN = (int)strlen(CIPHERTEXT);
    load_tetragrams(argv[1]);

    run();

    return 0;
}
