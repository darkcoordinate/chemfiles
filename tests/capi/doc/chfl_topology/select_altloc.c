// Chemfiles, a modern library for chemistry file reading and writing
// Copyright (C) Guillaume Fraux and contributors -- BSD license

#include <chemfiles.h>
#include <stdlib.h>
#include <assert.h>

int main() {
    // [example]
    CHFL_TOPOLOGY* topology = chfl_topology();

    CHFL_ATOM* a1 = chfl_atom("Cl");
    CHFL_ATOM* a2 = chfl_atom("Cl");
    CHFL_ATOM* a3 = chfl_atom("Cl");

    CHFL_PROPERTY* altlocA = chfl_property_string("A");
    CHFL_PROPERTY* altlocB = chfl_property_string("B");

    chfl_atom_set_property(a1, "altloc", altlocA);
    chfl_atom_set_property(a3, "altloc", altlocB);

    chfl_topology_add_atom(topology, a1);
    chfl_topology_add_atom(topology, a2);
    chfl_topology_add_atom(topology, a3);

    chfl_property_free(altlocA);
    chfl_property_free(altlocB);

    chfl_atom_free(a1);
    chfl_atom_free(a2);
    chfl_atom_free(a3);

    chfl_topology_free(topology);
    // [example]
    return 0;
}
