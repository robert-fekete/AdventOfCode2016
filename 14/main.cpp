#include <fstream>
#include <iostream>
#include <string>
#include <algorithm>
#include <map>
#include <sstream>
#include <vector>
#include <functional>
#include "MD5.h"

using namespace std;

int solve_first(string);
int solve_second(string);


int main(int argc, char* argv[])
{
    cout << solve_first("abc") << endl;  // 22728
    cout << solve_second("abc") << endl;  // 22551

    cout << solve_first("ihaygndm") << endl;
    cout << solve_second("ihaygndm") << endl;


	return 0;
}

char check_same_characters(string hash, int reoccurence = 3) {

    int index = 1;
    int last_char_num = 1;
    
    while (index < hash.size()) {
        if (hash[index] != hash[index - 1]) {
            if (last_char_num >= reoccurence) {
                return hash[index - 1];
            }
            last_char_num = 1;
        }
        else {
            ++last_char_num;
        }
        ++index;
    }
    if (last_char_num >= reoccurence) {
        return hash[index - 1];
    }
    return 0;
}

bool has_same_characters(string hash, char group_char, int reoccurence = 5) {

    int index = 1;
    int last_char_num = 1;
    while (index < hash.size()) {
        if (hash[index] != hash[index - 1]) {
            if (last_char_num >= reoccurence && hash[index - 1] == group_char) {
                return true;
            }
            last_char_num = 1;
        }
        else {
            ++last_char_num;
        }
        ++index;
    }
    return hash[index - 1] == group_char && last_char_num >= reoccurence;
}

int solve_with_hash(string salt, function<string(string)> hasher) {

    int offset = 0;
    int hash_iter = 0;
    int pads = 0;
    vector<string> hashes;

    while (pads < 64) {

        stringstream text;
        text << salt << offset;
        auto hash = hasher(text.str());
        hashes.push_back(hash);

        ++offset;

        if (offset >= 1000) {
            auto group = check_same_characters(hashes[hash_iter]);
            if (group != 0) {
                if (any_of(begin(hashes) + hash_iter + 1, begin(hashes) + hash_iter + 1000, [group](string hash) -> bool { return has_same_characters(hash, group); })) {

                    ++pads;
                }
            }
            ++hash_iter;
        }
    }

    return hash_iter - 1;
}

int solve_first(string salt){

    return solve_with_hash(salt, [](string text) -> string { return md5(text); });
}

int solve_second(string salt){

    return solve_with_hash(salt, [](string text) -> string {

        string temp = text;
        for (int i = 0; i < 2017; ++i) {
            temp = md5(temp);
        }

        return temp;
    });
}

