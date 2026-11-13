
#include "Adventure.h"
#include <iostream>
#include <sstream>

// ===================== GIVEN HELPERS =====================

bool Adventure::nextLine(std::ifstream& in, std::string& line) {
    std::string raw;
    while (std::getline(in, raw)) {
        int start = 0;
        while (start < (int)raw.size() &&
               (raw[start] == ' ' || raw[start] == '\t')) {
            start++;
        }
        line = raw.substr(start);

        if (!line.empty() && line[line.size() - 1] == '\r') {
            line = line.substr(0, line.size() - 1);
        }

        if (!line.empty()) {
            return true;
        }
    }
    return false;
}

bool Adventure::readField(std::ifstream& in,
                          const std::string& keyword,
                          std::string& value) {
    std::string line;

    if (!nextLine(in, line)) {
        std::cout << "Error: expected a " << keyword
                  << " line but the file ended." << std::endl;
        return false;
    }

    std::string prefix = keyword + " ";

    if (line.substr(0, prefix.size()) != prefix) {
        std::cout << "Error: expected a " << keyword
                  << " line but found: " << line << std::endl;
        return false;
    }

    value = line.substr(prefix.size());
    return true;
}

std::string Adventure::extendPath(
    const std::string& path, int choiceNumber) const {

    if (path.empty()) {
        return std::to_string(choiceNumber);
    }

    return path + "-" + std::to_string(choiceNumber);
}

// Helper: validate a whole integer.
static bool parseInteger(const std::string& text, int& number) {
    std::istringstream input(text);
    char extra;

    if (!(input >> number)) {
        return false;
    }

    return !(input >> extra);
}

// ===================== LOADING =====================

// TODO 10
bool Adventure::load(const std::string& fileName) {
    std::ifstream in(fileName);

    if (!in.is_open()) {
        std::cout << "Error: could not open file "
                  << fileName << std::endl;
        return false;
    }

    std::string value;
    int initialHp;

    if (!readField(in, "START_HP", value)) {
        return false;
    }

    if (!parseInteger(value, initialHp)) {
        std::cout << "Error: invalid START_HP." << std::endl;
        return false;
    }

    Scene<Moment>* root = readScene(in, "START");

    if (root == nullptr) {
        return false;
    }

    std::string extra;
    if (nextLine(in, extra)) {
        std::cout << "Error: unexpected extra story data."
                  << std::endl;
        delete root;
        return false;
    }

    startHp_ = initialHp;
    tree_.setRoot(root);

    return true;
}

// TODO 11
Scene<Moment>* Adventure::readScene(
    std::ifstream& in, const std::string& label) {

    std::string text;
    std::string hpText;
    std::string lootText;
    std::string item;
    std::string choicesText;

    if (!readField(in, "SCENE", text) ||
        !readField(in, "HP", hpText) ||
        !readField(in, "LOOT", lootText) ||
        !readField(in, "ITEM", item) ||
        !readField(in, "CHOICES", choicesText)) {

        return nullptr;
    }

    int hp;
    int loot;
    int numChoices;

    if (!parseInteger(hpText, hp) ||
        !parseInteger(lootText, loot) ||
        !parseInteger(choicesText, numChoices)) {

        std::cout << "Error: invalid scene number."
                  << std::endl;
        return nullptr;
    }

    if (numChoices < 0 ||
        numChoices > Scene<Moment>::MAX_CHOICES) {

        std::cout << "Error: scene \"" << text
                  << "\" has " << numChoices
                  << " choices. The limit is 3."
                  << std::endl;

        return nullptr;
    }

    Moment* moment = new Moment(
        text, label, hp, loot, item);

    Scene<Moment>* scene =
        new Scene<Moment>(moment, numChoices);

    for (int i = 0; i < numChoices; i++) {
        std::string choiceLabel;

        if (!readField(in, "LABEL", choiceLabel)) {
            delete scene;
            return nullptr;
        }

        Scene<Moment>* child =
            readScene(in, choiceLabel);

        if (child == nullptr) {
            delete scene;
            return nullptr;
        }

        scene->setChoice(i, child);
    }

    return scene;
}

// ===================== PLAYING =====================

// TODO 12
int Adventure::askChoice(int numChoices) {
    std::string input;
    int choice;

    while (true) {
        std::cout << "Your choice: ";

        if (!std::getline(std::cin, input)) {
            return 0;
        }

        if (parseInteger(input, choice) &&
            choice >= 1 && choice <= numChoices) {

            return choice;
        }

        std::cout << "Please type a number from 1 to "
                  << numChoices << "." << std::endl;
    }
}

// TODO 13
void Adventure::play() {
    if (tree_.isEmpty()) {
        return;
    }

    int hp = startHp_;
    int loot = 0;

    List<Item> bag;

    Scene<Moment>* current = tree_.getRoot();

    while (current != nullptr) {
        Moment* moment = current->getData();

        hp += moment->getHpChange();
        loot += moment->getLoot();

        std::cout << std::string(40, '-')
                  << std::endl;

        std::cout << moment->getText()
                  << std::endl;

        if (moment->hasItem()) {
            Item item(moment->getItem());

            if (bag.search(item)) {
                std::cout << "You already have: "
                          << item.getName()
                          << std::endl;
            } else {
                bag.addFront(
                    new Item(moment->getItem()));

                std::cout << "You picked up: "
                          << item.getName()
                          << std::endl;
            }
        }

        std::cout << "HP: " << hp
                  << "  Loot: " << loot
                  << std::endl;

        if (hp <= 0) {
            std::cout << "Your HP hit 0. GAME OVER."
                      << std::endl;
            break;
        }

        if (current->isEnding()) {
            std::cout << "THE END." << std::endl;
            break;
        }

        for (int i = 0;
             i < current->getNumChoices(); i++) {

            std::cout << "[" << i + 1 << "] "
                      << current->getChoice(i)
                             ->getData()->getLabel()
                      << std::endl;
        }

        int choice =
            askChoice(current->getNumChoices());

        if (choice == 0) {
            return;
        }

        current = current->getChoice(choice - 1);
    }

    std::cout << "Final HP: " << hp
              << "  Final loot: " << loot
              << std::endl;

    std::cout << "Inventory (" << bag.size()
              << " items):" << std::endl;

    bag.print();
}

// ===================== ANALYZING =====================

// TODO 14
void Adventure::printStats() const {
    std::cout << "Scenes: "
              << tree_.countScenes() << std::endl;

    std::cout << "Endings: "
              << tree_.countEndings() << std::endl;

    std::cout << "Longest playthrough: "
              << tree_.height()
              << " choices" << std::endl;
}

// TODO 15
void Adventure::printSpoilers() const {
    printEndings(tree_.getRoot(), "");
}

// TODO 16
void Adventure::printEndings(
    Scene<Moment>* scene,
    const std::string& path) const {

    if (scene == nullptr) {
        return;
    }

    if (scene->isEnding()) {
        std::cout << "Path "
                  << (path.empty() ? "(start)" : path)
                  << ": "
                  << scene->getData()->getText()
                  << std::endl;

        return;
    }

    for (int i = 0; i < scene->getNumChoices(); i++) {
        printEndings(
            scene->getChoice(i),
            extendPath(path, i + 1));
    }
}

// TODO 17
void Adventure::greedyRun() const {
    Scene<Moment>* current = tree_.getRoot();

    if (current == nullptr) {
        return;
    }

    int hp = startHp_;
    int loot = 0;
    std::string path;

    while (current != nullptr) {
        Moment* moment = current->getData();

        hp += moment->getHpChange();
        loot += moment->getLoot();

        if (hp <= 0 || current->isEnding()) {
            break;
        }

        int bestChoice = 0;

        for (int i = 1;
             i < current->getNumChoices(); i++) {

            if (current->getChoice(i)
                       ->getData()->getLoot() >
                current->getChoice(bestChoice)
                       ->getData()->getLoot()) {

                bestChoice = i;
            }
        }

        path = extendPath(path, bestChoice + 1);
        current = current->getChoice(bestChoice);
    }

    std::cout << "Greedy advisor" << std::endl;

    std::cout << "  Path: "
              << (path.empty() ? "(start)" : path)
              << std::endl;

    std::cout << "  Loot: " << loot
              << std::endl;

    if (hp <= 0) {
        std::cout << "  Result: died"
                  << std::endl;
    } else {
        std::cout << "  Result: reached an ending"
                  << std::endl;
    }
}

// TODO 18
int Adventure::bestLoot(
    Scene<Moment>* scene,
    int hp,
    const std::string& path,
    std::string& bestPath) const {

    if (scene == nullptr) {
        return -1;
    }

    Moment* moment = scene->getData();

    hp += moment->getHpChange();

    if (hp <= 0) {
        return -1;
    }

    int currentLoot = moment->getLoot();

    if (scene->isEnding()) {
        bestPath = path;
        return currentLoot;
    }

    int best = -1;

    for (int i = 0;
         i < scene->getNumChoices(); i++) {

        std::string childPath;

        int result = bestLoot(
            scene->getChoice(i),
            hp,
            extendPath(path, i + 1),
            childPath);

        if (result != -1) {
            int total = currentLoot + result;

            if (total > best) {
                best = total;
                bestPath = childPath;
            }
        }
    }

    return best;
}

// TODO 19
void Adventure::optimalRun() const {
    std::string bestPath;

    int loot = bestLoot(
        tree_.getRoot(),
        startHp_,
        "",
        bestPath);

    std::cout << "Optimal advisor" << std::endl;

    if (loot == -1) {
        std::cout
            << "  Result: no path survives this story"
            << std::endl;
        return;
    }

    std::cout << "  Path: "
              << (bestPath.empty() ? "(start)" : bestPath)
              << std::endl;

    std::cout << "  Loot: " << loot
              << std::endl;

    std::cout << "  Result: reached an ending"
              << std::endl;
}
