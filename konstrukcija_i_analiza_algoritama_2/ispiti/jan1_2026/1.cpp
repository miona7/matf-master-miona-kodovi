#include <iostream>
#include <map>
#include <vector>
#include <string>
#include <algorithm>

/*
    EDGE (ivica) u kompresovanom sufiksnom stablu.

    Ne čuvamo eksplicitno string na ivici,
    već samo interval [l, r) u originalnoj niskoj s.

    To znači:
        labela ivice = s[l], s[l+1], ..., s[r-1]
*/

struct Edge {
    int to;   // ID čvora do kog ova ivica vodi
    int l;    // početak podstringa u s (uključivo)
    int r;    // kraj podstringa u s (isključivo)
};

/*
    NODE (čvor) u sufiksnom stablu.

    next:
        mapa: početni karakter ivice -> odgovarajuća ivica

    suffix_index:
        koristi se samo kod listova
        predstavlja početnu poziciju sufiksa u originalnoj reči
        za unutrašnje čvorove je -1
*/

struct Node {
    int id;                              // identifikator čvora
    std::map<char, Edge> next;           // grane iz ovog čvora
    int suffix_index = -1;               // >= 0 samo za listove
};

/*
    Naivno kompresovano sufiksno stablo.
    Konstrukcija je O(n^2).
*/
struct SuffixTree {

    std::string s;        // originalna niska + '$'
    int orig_len = 0;     // dužina originalne niske bez '$'
    std::vector<Node> nodes;  // svi čvorovi stabla

    /*
        KONSTRUKTOR

        1) Dodaje terminalni znak '$' (ako ga nema).
           '$' mora biti karakter koji se ne pojavljuje u niskoj.

        2) Ubacuje sve sufikse jedan po jedan:
              s[0..]
              s[1..]
              s[2..]
              ...
    */
    SuffixTree(std::string str) : s(std::move(str)) {

        if(s.find('$') == std::string::npos) {
            s.push_back('$');
        }

        orig_len = (int)s.size() - 1;

        nodes.reserve(2 * (int)s.size() + 5);
        nodes.push_back(Node{0, {}, -1});   // čvor 0 = root

        // ubacujemo sve sufikse
        for(int i = 0; i < (int)s.size(); i++) {
            insert_suffix(i);
        }
    }

    /*
        Kreira novi čvor i vraća njegov ID.
    */

    int new_node() {
        int id = (int)nodes.size();
        nodes.push_back(Node{id, {}, -1});
        return id;
    }

    /*
        Računa LCP (Longest Common Prefix) dužinu
        između s[a..] i s[b..], ali najviše maxLen karaktera.

        Koristi se za poređenje duž ivice.
    */

    int lcp_limited(int a, int b, int maxLen) const {
        int k = 0;
        
        while(k < maxLen && a + k < (int)s.size() && b + k < (int)s.size() && s[a + k] == s[b + k]) {
            k++;
        }
        return k;
    }

    /*
        Ubacivanje jednog sufiksa koji počinje na poziciji "start".
    */

    void insert_suffix(int start) {
        int v = 0;        // počinjemo iz root-a
        int i = start;    // trenutna pozicija u stringu

        while(i < (int)s.size()) {
            char c = s[i];

            // Da li postoji grana koja počinje slovom c?
            auto it = nodes[v].next.find(c);

            // Ako ne postoji → dodajemo novu granu i list
            if(it == nodes[v].next.end()) {
                int leaf = new_node();
                nodes[leaf].suffix_index = start;

                nodes[v].next[c] = Edge{leaf, i, (int)s.size()};
                return;
            }

            // Ako postoji grana
            Edge e = it->second;

            int edgeLen = e.r - e.l;

            // Koliko se poklapa duž ivice?
            int k = lcp_limited(i, e.l, edgeLen);

            // Ako smo prešli celu ivicu → idemo dalje niz stablo
            if(k == edgeLen) {
                v = e.to;
                i += k;
                continue;
            }

            /*
                Ako je došlo do razlike u sred ivice,
                moramo da podelimo (split) ivicu.
            */

            int mid = new_node();

            // Skratimo staru ivicu da ide do mid
            nodes[v].next[c] = Edge{mid, e.l, e.l + k};

            // Stara ivica se nastavlja od mid
            Edge oldChild{e.to, e.l + k, e.r};
            nodes[mid].next[s[oldChild.l]] = oldChild;

            // Dodajemo novu granu za ostatak sufiksa
            int leaf = new_node();
            nodes[leaf].suffix_index = start;
            nodes[mid].next[s[i + k]] = Edge{leaf, i + k, (int)s.size()};

            return;
        }
    }

    /*
        DFS koji traži najdublji unutrašnji čvor.

        Zašto unutrašnji čvor znači ponavljanje?

        → Ako čvor ima bar dva potomka (nije list),
          znači da postoje bar dva sufiksa
          koji dele zajednički prefiks = substring
          koji odgovara putanji do tog čvora.

        depth = ukupna dužina putanje od korena do tog čvora
    */
    void longest_repeat_dfs(int v, int depth, int& best_len, int& best_node) const {

        //DOPUNA
        // unutrašnji čvor (nije root i nije list)
        

        // rekurzivno idemo kroz decu
        
    }

    /*
        Glavna funkcija za najduži ponovljeni segment.

        Vraća:
            {pozicija u originalnoj niskoj, dužina}
    */

    std::pair<int,int> longest_repeated_substring_pos_len() const {
        int best_len = 0;
        int best_node = -1;

        longest_repeat_dfs(0, 0, best_len, best_node);

        if(best_node == -1) {
            return {-1, 0};
        }

        /*
            Sada treba da pronađemo neku
            konkretnu poziciju tog substringa.

            Spuštamo se do nekog lista ispod best_node.
        */
        int v = best_node;
        while(!nodes[v].next.empty()) {
            v = nodes[v].next.begin()->second.to;
        }

        int start_pos = nodes[v].suffix_index;

        return {start_pos, best_len};
    }
};

int main() {
    std::string s;
    std::cout << "Unesi nisku: ";
    std::getline(std::cin, s);

    SuffixTree st(s);

    auto [pos, len] = st.longest_repeated_substring_pos_len();

    if(len == 0) {
        std::cout << "Nema ponovljenog segmenta.\n";
    } else {
        std::cout << "Najduzi ponovljeni segment:\n";
        std::cout << "Pocetak = " << pos << "\n";
        std::cout << "Duzina  = " << len << "\n";
        std::cout << s.substr(pos, len) << "\n";
    }

    return 0;
}
