#include "generator/figure.hpp"
#include "generator/plane.hpp"
#include <iostream>
#include <map>

using namespace std;

static char *PROGRAM_NAME;

// aqui no 1 elemento podia tar associado uma funcao, q seria chamada depois
// a string  seria o erro
static std::map<std::string, pair<int, string>> mapOptionsValues;

void printHelpPage() {
  cerr << "HELP PAGE: " << endl
       << "Usage: " << PROGRAM_NAME << " filename Option [Args ...]" << endl
       << "Options: " << endl;
  map<string, pair<int, string>>::iterator value;
  for (value = mapOptionsValues.begin(); value != mapOptionsValues.end();
       value++) {
    cerr << "\t" << value->first << endl;
  }
  cerr << "\t-help" << endl;
  cerr << "\t-h" << endl;
}

void initializeOptionsValues(int argc, char *argv[]) {
  int i = 1;
  mapOptionsValues["plane"] = {i, "PLANE!!"};
  mapOptionsValues["box"] = {++i, "BOX!!"};
  mapOptionsValues["sphere"] = {++i, "SPHERE!!"};
  mapOptionsValues["cone"] = {++i, "CONE!!!"};
  // mapOptionsValues["plane"] = {NULL, "PLANE!!"};
}

int main(int argc, char *argv[]) {
  initializeOptionsValues(argc - 3, argv + 3);
  PROGRAM_NAME = argv[0];

  if (argc < 3 || string(argv[1]) == "-h" || string(argv[1]) == "-help") {
    printHelpPage();
    return 1;
  }

  const std::string option = argv[2];

  if (mapOptionsValues.find(option) == mapOptionsValues.end()) {
    printHelpPage();
    return 1;
  }

  cerr << mapOptionsValues[option].second << "\n";

  return 0;
}
