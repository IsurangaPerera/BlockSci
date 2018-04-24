//
//  pubkey_range_py.hpp
//  blocksci
//
//  Created by Harry Kalodner on 4/23/18.
//
//

#ifndef blocksci_pubkey_range_py_h
#define blocksci_pubkey_range_py_h

#include "pubkey_py.hpp"
#include "address_py.hpp"
#include "caster_py.hpp"
#include "ranges_py.hpp"
#include "range_apply_py.hpp"

template <typename T, typename T2>
auto addPubkeyRange(pybind11::module &m, const std::string &name) {
    auto cl = addRangeClass<T>(m, name);
    addAddressMethods<blocksci::ScriptBase>(cl, [](auto func) {
        return applyMethodsToRange<T>(func);
    }, [](std::string docstring) {
        std::stringstream ss;
        ss << "For each pubkey: " << docstring;
        return strdup(ss.str().c_str());
    });
    addPubkeyBaseMethods<T2>(cl, [](auto func) {
        return applyMethodsToRange<T>(func);
    }, [](std::string docstring) {
        std::stringstream ss;
        ss << "For each pubkey: " << docstring;
        return strdup(ss.str().c_str());
    });
    return cl;
}

template <typename T, typename T2>
auto addOptionalPubkeyRange(pybind11::module &m, const std::string &name) {
    auto cl = addOptionalRangeClass<T>(m, name);
    addAddressMethods<blocksci::ScriptBase>(cl, [](auto func) {
        return applyMethodsToRange<T>(func);
    }, [](std::string docstring) {
        std::stringstream ss;
        ss << "For each pubkey: " << docstring;
        return strdup(ss.str().c_str());
    });
    addPubkeyBaseMethods<T2>(cl, [](auto func) {
        return applyMethodsToRange<T>(func);
    }, [](std::string docstring) {
        std::stringstream ss;
        ss << "For each pubkey: " << docstring;
        return strdup(ss.str().c_str());
    });
    return cl;
}

template <typename T>
void setupRanges(pybind11::module &m, const std::string &name) {
    std::stringstream ss;
    ss << "Any" << name << "Range";
    addPubkeyRange<ranges::any_view<T>, T>(m, ss.str());
    ss.clear();
    ss << name << "Range";
    addPubkeyRange<ranges::any_view<T, ranges::category::random_access>, T>(m, ss.str());
    ss.clear();
    ss << name << "AnyOptional" << name << "Range";
    addOptionalPubkeyRange<ranges::any_view<ranges::optional<T>>, T>(m, ss.str());
    ss.clear();
    ss << name << "Optional" << name << "Range";
    addOptionalPubkeyRange<ranges::any_view<ranges::optional<T>, ranges::category::random_access>, T>(m, ss.str());
}

void init_pubkey_range(pybind11::module &m);
void init_pubkeyhash_range(pybind11::module &m);
void init_witness_pubkeyhash_range(pybind11::module &m);
void init_multisig_pubkey_range(pybind11::module &m);


#endif /* blocksci_pubkey_range_py_h */