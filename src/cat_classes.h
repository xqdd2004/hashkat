#ifndef CAT_CLASSES_H_
#define CAT_CLASSES_H_

#include "cat_nodes.h"

/**
 * Convenience classes for forming classifier objects.
 */

namespace cats {

template <typename ElemT, typename CatData = std::vector<LeafNode<ElemT> >, int N_ELEMS = 1>
struct BaseLeafClass {
    typedef TreeNode< LeafNode<ElemT, N_ELEMS>, CatData > CatGroup;
    typedef typename CatGroup::iterator iterator;
};

template <typename ElemT, int MAX_CATS, int N_ELEMS = 1>
struct StaticBaseLeafClass : BaseLeafClass<ElemT, StaticVector<LeafNode<ElemT, N_ELEMS>, MAX_CATS>, N_ELEMS > {
};

template <typename ElemT, typename CatData = std::vector<LeafNode<ElemT> >, int N_ELEMS = 1>
struct LeafClass : BaseLeafClass<ElemT, CatData, N_ELEMS> {
    template <typename NetworkT>
    double& get(NetworkT& N, int bin) {
        return rates.at(bin);
    }
    void resize(int n) {
        rates.resize(n);
    }
    int size() {
        return rates.size();
    }
    void append(double rate) {
        rates.push_back(rate);
    }

    void fill_rates(std::vector<double>& rates) {
        this->rates = rates;
    }
    std::vector<double> rates;
};

template <typename ElemT, int MAX_CATS, int N_ELEMS = 1>
struct StaticLeafClass : LeafClass<ElemT, StaticVector<LeafNode<ElemT, N_ELEMS>, MAX_CATS>, N_ELEMS> {
};


template <typename InnerT, typename CatData = std::vector<typename InnerT::CatGroup>, int N_ELEMS = 1 >
struct TreeClass {
    typedef TreeNode<typename InnerT::CatGroup, CatData, N_ELEMS> CatGroup;
    typedef typename CatGroup::iterator iterator;

    template <typename NetworkT>
    InnerT& get(NetworkT& N, int bin) {
        return classes.at(bin);
    }
    void resize(int n) {
        classes.resize(n);
    }
    void append(InnerT& inner) {
        classes.push_back(inner);
    }

    int size() {
        return classes.size();
    }

    template <typename T>
    void fill_rates(std::vector<T>& rates) {
        resize(rates.size());
        for (int i = 0; i < classes.size(); i++) {
            classes[i].fill_rates(rates[i]);
        }
    }
    std::vector<InnerT> classes;
};

template <typename InnerT, int MAX_CATS, int N_ELEMS = 1>
struct StaticTreeClass : TreeClass<InnerT, StaticVector<typename InnerT::CatGroup, MAX_CATS>, N_ELEMS> {
};

}

#endif
