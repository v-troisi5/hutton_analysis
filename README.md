<link rel="stylesheet" href="https://jsdelivr.net">
<script defer src="https://jsdelivr.net"></script>
<script defer src="https://jsdelivr.net" onload="renderMathInElement(document.body);"></script>

(Spelling, naturalness of speech and writing in general is still in progress of being refined!)

## A Playful Analysis of the Puzzling Hutton (v1) Cipher

During the past weeks, I decided to tackle a small cryptographic challenge: I wanted to analyze, and hopefully attack, the Hutton cipher. If you are not familiar with the cipher's workings, allow me to redirect you to the Appendix.

Invented by **Eric Bond Hutton** in 2018, the cipher has been at the center of multiple challenges. I decided to try my hands at the first version of the system (known as **Hutton Cipher v1**).

Two points of note, before I continue:
- This is more of a fun experiment (a playful analysis to be exact!) rather than a scientific paper. I merely decided to have a go at this lovely puzzle, and then settled with writing a full report of my findings. I did my best to explain the logic behind every step of my process; still, I hope you will forgive the lacking formalism where it ought to have been used.
- My coding skills in Python and C need more than a bit of polishing for one and rusting for the other. It is highly probable you will find some code segments in which optimizations can and should be done. If you do, or if you have any questions or remarks at all, please feel free to contribute or to contact me!

### Initial Observations

If you've studied the workings of the Hutton v1 cipher (I will stop pointing out we're working on version 1 from now on), you may have instinctively said: "*That's a Vigenère cipher followed by Monoalphabetic Substitution!*" That's what I said anyway. It really isn't. There are two main points in which Hutton differs from a simple composition of the two:
- The initial alphabet is determined by a keyword. While this CAN be the case for a simple Aristocrat/Patristocrat, it usually isn't.
- The permutation of the alphabet on which the shift is applied changes (via a swap) every time a letter is encrypted.

We will refer to the shift keyword as $kw_1$ and to the mixing (alphabet) keyword as $kw_2$.

By tinkering with the cipher a bit, we can see that the two keys "protect" (**mask**) each other quite well. Not knowing the shift key prevents us from knowing what swaps have been executed at any given step. Not knowing the mixing key, prevents us from understanding the distance, i.e. the shift, between the plaintext letter and the ciphertext letter.

The cipher also exhibits a surprisingly strong "**avalanche effect**". Changing a single plaintext letter radically changes (most of) the ciphertext. Naturally, as per construction of the Hutton cipher, the avalanche can be detected only in the ciphertext that follows the changed letter. 

In essence, the continuous shifts and swaps render a ciphertext that it's akin to having been encrypted with a number of different substitution alphabets, two adjacent ones being separated by a single swap.

### Scope

The idea is to see if (and possibly how) we can reconstruct the shift key and/or the mixing key, given the ciphertext and/or additional information. As far as I know, the only other approach that has been applied to Hutton is the brute-force search of the keys or a dictionary attack on them. I wanted my approach to fit random keys as well. I should also mention I only considered texts written in English.

There is actually another approach by **Thomas Kaeding** (see the Appendix), who published a lovely paper in 2023 on the subject. It's a different approach, based on excluding plaintext candidates found by scanning Project Gutenberg. I guess I should have researched literature better, as I came to know about it only at the near end of this journey...

### Local Search
MysteryTwisterC3 (where I came to know about the cipher - see the Appendix) didn't sound exactly promising: the Hutton challenges (even the v1 ones) had been solved by brute-force, all other approaches having failed (before T. Kaeding's). Knowing that local search is an extremely common approach in classical cryptography, I wasn't exactly about to get my hopes up. Still, it was worth a shot.

I settled on the quadgram scoring function as a solution's fitness, so that the keys could be evaluated based on the score received by the plaintext they induced. The list was courtesy of **Practical Cryptography** (see the Appendix).

And... none of the approaches I tried worked. As I am writing this at the end of my project, I am also having a hard time remembering the countless roads I tried: hill climbing, simulated annealing, genetic algorithms in innumerable variants...

I tried optimizing $kw_1$ and $kw_2$ as a single key, I tried doing it separately, I tried adding and removing constraints on $kw_1$ and $kw_2$, I tried this and tried that, for every logically sound approach I could conceive, but none seemed to work. It wasn't really a surprise of course, but this strengthened my conviction that this cipher was marvelous in its simplicity.

### Further analysis: finding the length of $kw_1$ (```friedman.py```)
Suppose we take some time to settle back and analyze the cipher so that we can find out something more about the keys. For example, can I find out the length, or probably lengths, of $kw_1$?

(According to T. Kaeding, the answer is yes, but again, I didn't really do my research at this stage, so from my perspective, I had an open challenge to tackle.)

Okay, so, if the cipher even remotely resembles a Vigenère cipher in terms of shifting letters on an alphabet, then perhaps some of the classical approaches can work. I don't expect them to function perfectly due to the swaps that take place after the encryptions: still, it might be a good starting point.

The **Friedman Test** is a typical method used to find the length of a Vigenère key, based on the Index of Coincidence as defined by William F. Friedman himself. I considered the test could be valid as long as the underlying permutation of the alphabet **"doesn't change much"**, which is obviously a terribly informal way to put it.

Diaconis & Shahshahani proved in 1981 that, given a string of $n$ characters, the number of random swaps of two characters it takes for the string to appear random is roughly $\frac{n}{2}\ln(n)$. With $n = 26$ in our case, it takes around $42$ steps.

Therefore, I divided the ciphertext is blocks of $42$ characters and ran the Friedman Test on each. The shift doesn't change between blocks, but the alphabet does, so my hope was to get a coherent aggregate result. For the vast majority of tests I've run, the correct length of the key, or a multiple of it, was in the upper top 5 of the results. Therefore, from this point on, I assumed to have access to the length of $kw_1$, as every consequent step could, in theory, be done on a few plausible candidates.

**Side note**: thinking back on it, this is a curious result. Theoretically, the Index of Coincidence should depend on the frequency of the characters, which isn't be a robust measure with the Hutton cipher as it changes immediately for the encrypted character and its substitute. Still, the results are very promising. Reasoning through it, I suspect it works because a given plaintext character may return to its own original ciphertext correspondent one in a period that depends on the length of $kw_1$ and with a probability that, a priori, it's the same for every character. Therefore, it's "as if" the character's frequencies get modified by a normalization constant which averages the same for every character in the ciphertext, thus keeping the relative frequencies, and the comparisons of the IC values, intact.

### Finding the length of $kw_2$? (```alphabet_kw_length.py```)
This stage was way less formal than the previous, I'm afraid. Wanting to find the length of $kw_2$ as well, I decided to run a simulated annealing algorithm that worked by:
- Randomly perturbing $kw_1$ or $kw_2$;
- Restarting for each plausible length of $kw_1$.

Now, we will see how very little fitness information can be gained by just looking at $kw_2$ (**given its fixed length**). However, at this stage, I hadn't run this analysis yet, and I merely thought that a mixing keyword of proper length is such that, on average, less alphabetical characters are farther from their true position in the lexicographically ordered $A...Z$ sequence. Thus, observing higher fitness results for key lengths closer to the true one, I decided to run my further attacks assuming I knew (or could try from a small set of candidates) the lengths of $kw_1$ and $kw_2$. Admittedly this step has been severely under-tested with respect to the previous one; regardless, I made this my starting point.

Note: the keys for every example cipher you can now find in the source file are $kw_1$ = HILL and $kw_2$ = VALLEY (which is rendered as VALEY in the initial alphabet). I used a lot of exemplifying ciphertexts (and keys) in my testing, which I either found online (and I don't really remember where - again, this is but a playful experiment from my side) or produced them myself. Please feel free to test the approach more!

### Swarm Intelligence on a reduced problem (```hutton_abc_fixed.c```)
Since I needed a new approach, I decided to try an idea I hadn't explored as much as the more popular ones like genetic algorithms: **Swarm Intelligence**.

Swarm Intelligence is a class of metaheuristics used in optimization that consider a problem as an environment that can be explored by simple agents. These agents can develop new solutions, interact with their environment and communicate with one another to reach an optimal solution. This category of algorithms is usually inspired by naturally occurring phenomena of these kind (like the coordination behind an ant colony or a flock of birds).

I settled on the **Artificial Bee Colony (ABC)** algorithm which I had had a bit more experience with before, compared to others. This was also the point in which I had to start coding in C, as the Python version I originally developed ran way too slowly.

The idea behind the ABC variant I used was to employ different squadrons of agents (bees):
- **Employed bees** consider a food source (i.e. a solution consisting mainly of $kw_1$ and $kw_2$ and the fitness score of the relative plaintext) and explore its neighborhood by choosing one of two keys and either substituting a character for another in the alphabet or swapping two characters.
- **Onlooker bees** use roulette wheel selection to filter solutions provided by the employed bees, and keep exploring the neighborhood with the same logic.
- Food sources that haven't improved their score after a limit of subsequent steps are replaced with new random sources (this is the "**scouting phase**").

Side note: I implemented dozens of versions of the ABC algorithm in order to find out what could work: the final results I'm presenting stem from slightly different variants, so you may find minor differences in the code. The underlying logic, however, stays the same. Again, should you find any leftover issues, please don't hesitate to contact me!

Anyway, after restarting multiple times and allowing thousands of cycles to run, again, no solution would be reached.

I also tried sweeping over the alphabet characters of $kw_1$ and $kw_2$ to see if fixing some characters scored better on average: no luck. I tried implementing this approach allowing for more optimization cycles to be executed in later steps, when more characters were fixed. To reduce the size of the problem, I tried implementing multiple alternating "passes" of this alphabet-reduction strategy. The idea was for the characters of a key to be fixed by only sampling keys from the set of the best character candidates for the other key. Such a set would be fixed during the previous iteration. Again, no luck. And again, this is but an extremely concise summary of the approaches I tried. 

Not only that, but the quadgram score would only improve by mere crumbs.

Therefore, I decided to work on a smaller problem first. What if we suppose to know one of the keys? It turns out that the ABC strategy can indeed find one of the keys when given the other. Given $kw_1$, $kw_2$ can be broken. Given $kw_2$, you can break $kw_1$. Cycles take longer, the longer the ciphertext, but with a fixed key, most of my experiments yielded the correct solution in $\le 500$ cycles.

I suspect this same result could be achieved with simpler local search approaches (compared to ABC) and again, this seems to be an indication of how the two keys "protect" each other. However, the cipher crumbles when SOME relevant information is known (duh) and this reduced version was the very first "breakthrough" of the project.

### Quadgram Fitness Landscape (```hutton_landscape_kl.py```)
When I observed how little the quadgram score was improving, even when getting very close to the actual keys, I ran a handful of experiments.

First, I decided to fix single characters of $kw_1$/$kw_2$ and run ABC many times for each, averaging the best results. To my dismay, most fitness values were so similar to be considered (almost) the same, especially when constraining $kw_2$. 

Intuitively, this served as a clue to the fact that the quadgram scoring landscape, with respect to Hutton cipher, was extremely flat, with roughly only a single peak corresponding to the global optimum (i.e. the correct ($kw_1$,$kw_2$) pair). This result stayed the same when I opted for using **Kullback-Leibler Divergence** between the expected distribution of quadgrams in English and the actual distribution of quadgrams in the ciphertext.

I analyzed the landscape of such fitness in more detail in order to gain insight on what is actually doable to recover the key. I considered:
- **Principal Component Analysis**: considering each key as a point in a $|kw_1|+|kw_2|$-dimensional space. 2D projection resulted to be representing only $\approx 22\%$ of the variance.
- **Fitness Distance Correlation**: measuring the linear correlation between a solution's fitness and its (Hamming) distance from the known best. Pearson's correlation resulted to be $\approx -0.015$ slightly negative on average, but obviously nil for most purposes. 
- **Autocorrelation of the fitness sequence**: executing independent random walks and perturbing the keys at each step. Correlation length happened to average at around $1$ step, thereby confirming the avalanche effect we had informally observed.
- **Mutual Information** between each position of the key(s) and the fitness value: this would capture all correlation, not just linearity. The result averaged to be $\approx 0.016$ nats, or $\approx 0.023$ bits. The correlation appeared to be slightly higher between the fitness and $kw_1$, rather than $kw_2$.
- **Fitness Differential (per position)**: sampling random keys and swapping out any character of $kw_1$ or $kw_2$ for a random other predictably produced minimal fitness differences. I say predictably, as it was naturally guessable from the MI analysis.
- **Round-by-Round Correlation**: comparing how the fitness value evolves during rounds with respect to the one associated to the known best key. The separation between the best and the random curves was very distinct, suggesting that the KL metric is indeed distinguishing the best from the rest well. However, given the MI from the previous steps, much more ciphertext material may be needed to find out the keys.

Nevertheless, the analysis showed that we have very little information to play with, as the strong "**coupling**" of the two keys renders the ciphertext very difficult to attack with a key-recovering strategy. That is also why the local search approaches (at least those based on quadgram scoring, although I tried different fitness functions and essentially got the same result) fail: no algorithm can build or recover information that isn't there in the first place.

Therefore, unless we use a completely different fitness function (one I haven't thought of!), we only have two ways of executing a key-recovery attack, with our assumptions and scope. Either we exploit the little information we can gain (mostly about $kw_1$) to run a reduced brute-force, or we can change our adversarial model.

### Reduced brute-force attack (```hutton_topk.c```)
The idea is simple: probe a number of random keys and average their fitness conditioned on fixing a specific character in a specific position of $kw_1$. We keep the top $k$ results for each character.

Afterward, we brute-force the $k^{|kw_1|}$ dispositions and run ABC on each. We can tweak the ABC parameters to make sure the executions is sensibly fast for each brute-forced disposition.

Naturally, remembering our previous experiments, we know that not much information can be gleaned from the fitness. Thus, we'll need a value of $k$ that may be closer to $26$ than what we expect. During my experiments (which were numerous, but by no means exhaustive), I found out that values of $k$ between $13$ and $24$ returned a candidate set with the correct character for that position in it.

Using ABC is faster than brute-forcing long mixing keys as well, so our reduced brute-forcing is all in all a faster approach than naivety to solve Hutton v1. Of course, random, one-time shift keys with length comparable to the ciphertext are harder to break (and in particular, if the lengths match, Hutton cipher is essentially a One-Time Pad, in the same way Vigenère behaves with those same assumptions).

### KPA/CPA Approach
Now, let us suppose to have access to a discretely long **crib**, at least with respect to $|kw_1|$ (the same approach can be replicated even more accurately if we can access an oracle that encrypts text for us with the keys we're trying to find out).

Let us formalize the problem in the following way:
- We consider plaintext $m_1...m_n$ of length $n$;
- Encrypted with Hutton v1 in ciphertext $c_1...c_n$;
- Using shift key $p_1...p_t$;

According to our previous results, we assume $n,t$ to be known. Furthermore, we formalize the mixing key as:
- $k_A...k_Z$, where $k_{\sigma}$ is the circular shift (mod $26$) applied on character $\sigma$ in order to obtain its current position in the mixed alphabet.

For example, the alphabet VALEYBCDFGHIJKMNOPQRSTUWXZ, with $kw_2 = $ VALLEY, is such that $k_V = -21 \mod 26 = 5 \mod 26$, since $kw_2$ should stand in position $21$ in the lexicographically ordered alphabet ABCDEFGHIJKLMNOPQRSTUVWXYZ and now stands in position $0$ in the mixed alphabet: working mod $26$, going $21$ characters back is the same as going $5$ forwards.

Therefore, we can describe the encryption of character $m_i$ with the following two operations:
- $c_i +k_{c_i} = m_i + k_{m_i} + p_{i \text{ mod } t}$ , i.e., $c_i \leftarrow m_i + k_{m_i} + p_{i \text{ mod } t} - k_{c_i}$
- $k_{m_i}$ and $k_{c_i}$ are swapped;

The swap can be formalized by observing that the current shift $k_{m_i}$ is increased by $c_i + k_{c_i} - m_i - k_{m_i}$ thereby becoming $k_{c_i} + (c_i - m_i)$. By the same reasoning, $k_{c_i}$ becomes $k_{m_i} - (c_i - m_i)$, although we don't need this now (!)

Of course, we are referring to $m_i$ and $c_i$ as their positions in the alphabet $A...Z$.

Now, suppose we are given two ciphertext characters $c_i, c_j$ matching two crib characters $m_i, m_j$ we know:
- $c_i = m_i + k_{m_i} + p_{i \text{ mod } t} - k_{c_i}$
- $c_j = m_j + k_{m_j} + p_{j \text{ mod } t} - k_{c_j}$

Subtracting one from the other we get:
- $c_i - c_j = m_i + k_{m_i} + p_{i \text{ mod } t} - k_{c_i} - m_j - k_{m_j} - p_{j \text{ mod } t} + k_{c_j}$

Re-arranging:
- $(c_i - c_j) - (m_i + m_j) = (p_{i \text{ mod } t}- p_{j \text{ mod } t}) + k_{m_i} - k_{m_j} + k_{c_j} - k_{c_i}$

With enough observations and a ciphertext alphabet that gets random fast, the differences $k_{m_i} - k_{m_j}$ and $k_{m_i} - k_{m_j}$ have **constant expectation** thereby making our equality:
- $(c_i - c_j) - (m_i + m_j) \approx (p_{i \text{ mod } t}- p_{j \text{ mod } t}) + C$

Since we have a crib $m_i, m_j$ are known as well as $c_i, c_j$. Therefore, we can obtain information on the difference in shift $(p_{i \text{ mod } t}- p_{j \text{ mod } t})$ by exploiting the above relationship.

### Recovering information on the shift key (```hutton_kpa.py```)
Stripping away the implementation details and pains, the idea is what we formalized in the previous section.

We consider every pair $(i,j), i < j$ belonging to the crib and compute the observation $obs = (c_i - c_j) - (m_i + m_j)$ for each of them. This is accumulated in the dictionary entry $obs[(i \text{ mod } t, j \text{ mod } t)]$. We also compute the circular mean and standard deviation at this stage.

As a secondary output, we can also attempt to remove the constant $C$ by pivoting on each computed mean $m(\cdot,\cdot)$. For each triple $(i,j,k)$ of circular indexed, we compute $m(i,j) - m(i,k) \approx (p_{i \text{ mod } t}- p_{j \text{ mod } t} + C_{ij}) - (p_{i \text{ mod } t}- p_{k \text{ mod } t} + C_{ik}) \approx (p_{k \text{ mod } t}- p_{j \text{ mod } t})$.

As again, with enough data, all values $C_{\cdot,\cdot}$ converge to the same constant value $\approx \frac{26}{3}$.

In practice, this approach mostly returned, even with a crib that is relatively short compared to the ciphertext length, constraints of the shift differences with a small integer error (this, remembering that the shift is circular), which can be enough to exclude a significant number of possible shift keys. In any case, this approach allows us to glean information about the shift key and a refined version of it could prove even more useful for practical purposes.

### Conclusions

Studying the Hutton Cipher has been both stimulating and fascinating. I really enjoyed working on this problem, and I'm happy to have achieved at least some small results on the matter. Nevertheless, this which began as a "playful analysis" has been a true pleasure to bring to a conclusion, whether temporary or permanent.

Again, please excuse me if you happen to find any mistakes or inaccuracies. If you have any suggestions, remarks, or proposals, feel free to contact me and I'll do my best to follow them! And of course, thank you if you've read this far. I truly appreciate it!


#### Appendix

##### How the Hutton cipher works:
- https://en.wikipedia.org/w/index.php?title=User:Eric_Bond_Hutton&oldid=840686562
- https://hutton-cipher.netlify.app/howto.html

##### Online simulator for Hutton cipher (both v1 and v2):
- https://hutton-cipher.netlify.app/

##### Some Hutton cipher challenges (more exist!) including the solution to the first challenge (on Reddit):
- https://www.reddit.com/r/cryptography/comments/8mozl5/hutton_cipher/
- https://mysterytwister.org/challenges?search=hutton

##### T. Kaeding's Approach:
- https://eprint.iacr.org/2023/1113.pdf

##### Practical Cryptography's page for n-grams lists:
- http://practicalcryptography.com/cryptanalysis/letter-frequencies-various-languages/

##### Friedman Test:
- https://en.wikipedia.org/wiki/Vigen%C3%A8re_cipher#Friedman_test

##### On Random Transpositions:
- https://www.imo.universite-paris-saclay.fr/~pierre-loic.meliot/symmetric/texts/Diaconis,%20Shahshahani%20-%20Generating%20a%20random%20permutation%20with%20random%20transpositions.pdf

##### Artificial Bee Colony (ABC):
- https://en.wikipedia.org/wiki/Artificial_bee_colony_algorithm

##### Information Theory Fundamentals:
- https://en.wikipedia.org/wiki/Entropy_(information_theory)
- https://en.wikipedia.org/wiki/Kullback%E2%80%93Leibler_divergence
- https://en.wikipedia.org/wiki/Mutual_information
