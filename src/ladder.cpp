#include "ladder.h"

void error(string word1, string word2, string msg) {
    cout << "Error: " << msg << " (" << word1 << " -> " << word2 << ")" << endl;
}

bool edit_distance_within(const std::string& str1, const std::string& str2, int d) {
    int len1 = str1.size(), len2 = str2.size();
    
    // If the absolute length difference is greater than d, return false early
    if (abs(len1 - len2) > d) return false;

    vector<vector<int>> dp(len1 + 1, vector<int>(len2 + 1, 0));
    // Initialize base cases
    for (int i = 0; i <= len1; ++i) dp[i][0] = i; // Deleting all characters
    for (int j = 0; j <= len2; ++j) dp[0][j] = j; // Inserting all characters

    // Compute DP table
    for (int i = 1; i <= len1; ++i) {
        for (int j = 1; j <= len2; ++j) {
            if (str1[i - 1] == str2[j - 1]) {
                dp[i][j] = dp[i - 1][j - 1]; // No change needed
            } else {
                dp[i][j] = min({dp[i - 1][j] + 1,    // Deletion
                                dp[i][j - 1] + 1,    // Insertion
                                dp[i - 1][j - 1] + 1 // Substitution
                });
            }
        }
    }
    return dp[len1][len2] <= d;
}


bool is_adjacent(const string& word1, const string& word2) {
    int len1 = word1.size(), len2 = word2.size();
    
    // Special case: Identical words should be considered adjacent
    if (word1 == word2) return true;

    if (abs(len1 - len2) > 1) return false; // More than 1 edit = not adjacent

    int diff_count = 0;
    int i = 0, j = 0;

    while (i < len1 && j < len2) {
        if (word1[i] != word2[j]) {
            diff_count++;
            if (diff_count > 1) return false; // More than 1 change is not allowed
            
            // Handle insertion/deletion case (skip one character)
            if (len1 > len2) i++;  // Deletion
            else if (len1 < len2) j++;  // Insertion
            else { i++; j++; }  // Substitution case
        } else {
            i++; j++;
        }
    }
    
    // Account for cases where last character is added/removed
    if (i < len1 || j < len2) diff_count++;

    return diff_count == 1;
}


vector<string> generate_word_ladder(const string& begin_word, const string& end_word, const set<string>& word_list) {
    if (begin_word == end_word) {
        error(begin_word, end_word, "Start and end words are the same!");
        return {}; // Return an empty vector to avoid infinite loop
    }

    queue<vector<string>> ladder_queue;
    set<string> visited;

    ladder_queue.push({begin_word});
    visited.insert(begin_word);

    while (!ladder_queue.empty()) {
        vector<string> ladder = ladder_queue.front();
        ladder_queue.pop();
        string last_word = ladder.back();

        for (const string& word : word_list) {
            if (is_adjacent(last_word, word) && visited.find(word) == visited.end()) {
                vector<string> new_ladder = ladder;
                new_ladder.push_back(word);
                visited.insert(word);

                if (word == end_word) return new_ladder;
                ladder_queue.push(new_ladder);
            }
        }
    }
    return {}; // No ladder found
}


#include <algorithm> // For transform()

void load_words(set<string>& word_list, const string& file_name) {
    ifstream file(file_name);
    if (!file) throw runtime_error("Cannot open word list file.");

    string word;
    while (file >> word) {
        transform(word.begin(), word.end(), word.begin(), ::tolower); // Convert to lowercase
        word_list.insert(word);
    }
    file.close();
}

void print_word_ladder(const vector<string>& ladder) {
    if (ladder.empty()) {
        cout << "No word ladder found." << endl;
        return;
    }
    
    cout << "Word ladder found: ";
    for (const string& word : ladder) {
        cout << word << " ";
    }
    cout << endl;
}




#define my_assert(e) {cout << #e << ((e) ? " passed" : " failed") << endl;}

void verify_word_ladder() {
    set<string> word_list;
    load_words(word_list, "words.txt");

    my_assert(generate_word_ladder("cat", "dog", word_list).size() == 4);
    my_assert(generate_word_ladder("marty", "curls", word_list).size() == 6);
    my_assert(generate_word_ladder("code", "data", word_list).size() == 6);
    my_assert(generate_word_ladder("work", "play", word_list).size() == 6);
    my_assert(generate_word_ladder("sleep", "awake", word_list).size() == 8);
    my_assert(generate_word_ladder("car", "cheat", word_list).size() == 4);
}