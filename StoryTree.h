
#pragma once
#include "Scene.h"

// The whole story. It knows the first scene and nothing else.
//
// RULES (do not change):
// - Keep every name and signature exactly as written.
// - The private helpers must be recursive.
// - This class must work for ANY T.

template <typename T>
class StoryTree {
private:
    Scene<T>* root_;

    // TODO 6
    int countScenes(Scene<T>* scene) const {
        if (scene == nullptr) {
            return 0;
        }

        int total = 1;

        for (int i = 0; i < scene->getNumChoices(); i++) {
            total += countScenes(scene->getChoice(i));
        }

        return total;
    }

    // TODO 7
    int countEndings(Scene<T>* scene) const {
        if (scene == nullptr) {
            return 0;
        }

        if (scene->isEnding()) {
            return 1;
        }

        int total = 0;

        for (int i = 0; i < scene->getNumChoices(); i++) {
            total += countEndings(scene->getChoice(i));
        }

        return total;
    }

    // TODO 8
    int height(Scene<T>* scene) const {
        if (scene == nullptr) {
            return -1;
        }

        int maxHeight = -1;

        for (int i = 0; i < scene->getNumChoices(); i++) {
            int childHeight = height(scene->getChoice(i));

            if (childHeight > maxHeight) {
                maxHeight = childHeight;
            }
        }

        return maxHeight + 1;
    }

public:
    StoryTree() : root_(nullptr) {}

    ~StoryTree() {
        clear();
    }

    void setRoot(Scene<T>* root) {
        clear();
        root_ = root;
    }

    Scene<T>* getRoot() const {
        return root_;
    }

    bool isEmpty() const {
        return root_ == nullptr;
    }

    // TODO 9
    void clear() {
        delete root_;
        root_ = nullptr;
    }

    int countScenes() const {
        return countScenes(root_);
    }

    int countEndings() const {
        return countEndings(root_);
    }

    int height() const {
        return height(root_);
    }
};
