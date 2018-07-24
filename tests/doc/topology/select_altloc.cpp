// Chemfiles, a modern library for chemistry file reading and writing
// Copyright (C) Guillaume Fraux and contributors -- BSD license
#include <catch.hpp>
#include <chemfiles.hpp>
using namespace chemfiles;

#undef assert
#define assert CHECK

TEST_CASE() {
    // [example]
    auto a1 = Atom("Cl");
    auto a2 = Atom("Cl");
    auto a3 = Atom("Cl");
    a1.set("altloc", "A");
    a2.set("altloc", "B");

    auto topology = Topology();
    topology.add_atom(std::move(a1));
    topology.add_atom(std::move(a2));
    topology.add_atom(std::move(a3));

    assert(topology.size() == 3);
    topology.select_altloc("A");
    assert(topology.size() == 2);
    // [example]
}
