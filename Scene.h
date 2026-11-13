
#pragma once

template <typename T>
class Scene {
private:
    T* data_;
    Scene<T>** choices_;
    int numChoices_;

public:
    static const int MAX_CHOICES = 3;

    // TODO 1
    Scene(T* data, int numChoices)
        : data_(data), choices_(nullptr), numChoices_(numChoices) {

        if (numChoices_ > 0) {
            choices_ = new Scene<T>*[numChoices_]();
        }
    }

    // TODO 2
    ~Scene() {
        for (int i = 0; i < numChoices_; i++) {
            delete choices_[i];
        }

        delete[] choices_;
        delete data_;
    }

    T* getData() const {
        return data_;
    }

    int getNumChoices() const {
        return numChoices_;
    }

    // TODO 3
    bool isEnding() const {
        return numChoices_ == 0;
    }

    // TODO 4
    Scene<T>* getChoice(int index) const {
        if (index < 0 || index >= numChoices_) {
            return nullptr;
        }

        return choices_[index];
    }

    // TODO 5
    void setChoice(int index, Scene<T>* next) {
        if (index >= 0 && index < numChoices_) {
            choices_[index] = next;
        }
    }
};
