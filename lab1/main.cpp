#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <vector>
#include <set>



bool is_letter(char c) {
  return 'a' <= c && c <= 'z' || 'A' <= c && c <= 'Z';
}

bool isUnique(long long &mask, char c){
    if ('a' <= c && c <= 'z') {
        int pos = c - 'a';
        if ((mask >> pos)&1) return false; else
            mask |= (1 << pos);
    } else if ('A' <= c && c <= 'Z') {
        int pos = c - 'A' + ('z' - 'a' + 1);
        if ((mask >> pos)&1) return false; else
            mask |= (1 << pos);
    } 
    return true;
}

int main()  {
  FILE *fp;
  const int LEN = 256;
  char buffer[LEN + 1];
  const char* file_name = "lab1.txt";
  fp = fopen(file_name, "r");
  //fp = stdin;
  size_t r;
  std::vector<std::string> vs;
  std::string current;

  while ((r = fread(buffer, 1, LEN, fp)) > 0) {
    for (int i = 0; i < r; ++i) {
      char c = buffer[i];
      if (is_letter(c)) {
        current += c;
      } else {
        if (!current.empty()) {
          vs.push_back(current);
        }
        current.clear();
      }
    }
  }
  if (!current.empty()) {
      //r = fread(buffer, 1, LEN, fp);
    vs.push_back(current);
  }
  current.clear();

  long long mask = 0;
  std::set<std::string> S;
  for (std::string s : vs) {
    mask = 0;
    bool ok = true;
    for (char c : s) {
      if (!isUnique(mask, c)) {
        ok = false;
        break;
      }
    }
    if (ok) {
      S.insert(s);
    }
  }
  printf("%d\n", (int)S.size());
  for (std::string s : S) {
    printf("%s\n", s.c_str());
  }

  fclose(fp);

  return(0);
}