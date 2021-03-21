//
// Created by nrx on 06.08.2020.
//

#ifndef TESTSIMULINKMODEL_FORMATSCONTAINER_H
#define TESTSIMULINKMODEL_FORMATSCONTAINER_H
#include "format/Format.h"
#include <unordered_set>
#include <memory>

template<typename  T>
class FormatsContainer {
public:
    struct FormatHash {
        size_t operator()(const std::shared_ptr<Format<T>> &format) const {
            return static_cast<size_t>(qHash(format->name()));
        }
    };
    void add(const std::shared_ptr<Format<T>> &format) {
        container.push_back(format);
     }
     std::shared_ptr<Format<T>> get(const QString &formatName) {
        auto ptr = std::find_if(container.begin(), container.end(),
                     [&](const std::shared_ptr<Format<T>> &el) {
            return el->name() == formatName;
        });
        if (ptr == container.end()) {
            return nullptr;
        }
         return *ptr;
     }
     QStringList names() {
         QStringList res;
         for(const auto &el : container) {
             res.append(el->name());
         }
         return res;
     }
private:
    // std::unordered_set<std::shared_ptr<Format<T>>, FormatHash> container;
    std::vector<std::shared_ptr<Format<T>>> container;
};


#endif //TESTSIMULINKMODEL_FORMATSCONTAINER_H
