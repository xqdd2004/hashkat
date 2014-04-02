#ifndef FOLLOWERSET_H_
#define FOLLOWERSET_H_

#include <cstdio>
#include <cmath>
#include "util.h"

#include "util/FlexibleSet.h"

// For bin limits:
#include "config_static.h"

// Forward declarations to prevent circular imports:
struct Entity;
struct Tweet;

/*****************************************************************************
 * Categorization layers of the follower set:
 *****************************************************************************/

// Leaf layer
struct IdeologyLayer {
    static const int N_SUBLAYERS = N_BIN_IDEOLOGIES;

    struct Weights {
        double weights[N_SUBLAYERS] = {0};
    };

    struct WeightDeterminer {
        double humour_bins[N_BIN_HUMOUR] = {1, 1};
    };

    static int classify(Entity& entity);

    int n_elems = 0; // Total
    FlexibleSet<int> sublayers[N_SUBLAYERS];
};

struct RegionLayer {
    typedef IdeologyLayer ChildLayer;
    static const int N_SUBLAYERS = N_BIN_REGIONS;

    struct Weights {
        ChildLayer::Weights subweights[N_SUBLAYERS];
        double weights[N_SUBLAYERS] = {0};
    };

    struct WeightDeterminer {
        double humour_bins[N_BIN_HUMOUR];
    };

    static int classify(Entity& entity);

    int n_elems = 0; // Total
    ChildLayer sublayers[N_SUBLAYERS];
};

struct PreferenceClassLayer {
    typedef RegionLayer ChildLayer;
    static const int N_SUBLAYERS = N_BIN_PREFERENCE_CLASS;

    struct Weights {
        ChildLayer::Weights subweights[N_SUBLAYERS];
        double weights[N_SUBLAYERS] = {0};
    };

    struct WeightDeterminer {
        double humour_bins[N_BIN_HUMOUR];
    };

    static int classify(Entity& entity);

    int n_elems = 0; // Total
    ChildLayer sublayers[N_SUBLAYERS];
};

// Top layer
struct LanguageLayer {
    typedef PreferenceClassLayer ChildLayer;
    static const int N_SUBLAYERS = N_LANGS;

    struct Weights {
        ChildLayer::Weights subweights[N_SUBLAYERS];
        double weights[N_SUBLAYERS] = {0};
    };

    struct WeightDeterminer {
        ChildLayer::WeightDeterminer preference_class_bins[N_BIN_HUMOUR];
    };

    static int classify(Entity& entity);

    int n_elems = 0; // Total
    ChildLayer sublayers[N_SUBLAYERS];
};

/*****************************************************************************
 * Implementation of the follower set:
 *****************************************************************************/

struct FollowerSet {
    typedef LanguageLayer TopLayer;
    typedef TopLayer::Weights Weights;
    typedef TopLayer::WeightDeterminer WeightDeterminer;

    std::vector<int> as_vector();

    /* Returns false if the element already existed */
    bool add(Entity& entity);

    /* Returns true if the element already existed */
    bool remove(Entity& entity);

    /* Returns an element, provided the given weights */
    bool pick_random_weighted(MTwist rng, Weights& weights, int& id);

    /* Returns an element, weighing all options equally */
    bool pick_random_uniform(MTwist& rng, int& id);

    void print();

    size_t size() const {
        return followers.n_elems;
    }

    void determine_tweet_weights(WeightDeterminer& determiner, Tweet& tweet, /*Weights placed here: */ Weights& output);

private:
    // Holds the actual followers:
    TopLayer followers;
};


#endif /* FOLLOWERSET_H_ */
