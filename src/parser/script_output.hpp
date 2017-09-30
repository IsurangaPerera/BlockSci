//
//  script_output.hpp
//  blocksci
//
//  Created by Harry Kalodner on 3/18/17.
//
//

#ifndef script_output_hpp
#define script_output_hpp

#include "basic_types.hpp"

#include <blocksci/scripts/bitcoin_script.hpp>
#include <blocksci/scripts/bitcoin_pubkey.hpp>
#include <blocksci/address/address_info.hpp>
#include <blocksci/address/address.hpp>

#include <boost/variant/variant_fwd.hpp>

#include <array>

class AddressWriter;
class AddressState;

template<blocksci::AddressType::Enum type>
struct ScriptOutput;

template <>
struct ScriptOutput<blocksci::AddressType::Enum::PUBKEY> {
    CPubKey pubkey;
    
    ScriptOutput(const std::vector<unsigned char> &vch1);
    ScriptOutput(const CPubKey &pub) : pubkey(pub) {}
    
    void processOutput(AddressState &, AddressWriter &writer);
    void checkOutput(const AddressState &, const AddressWriter &) {}
    
    bool isValid() const { return true; }
    
    blocksci::uint160 getHash() const;
};

template <>
struct ScriptOutput<blocksci::AddressType::Enum::PUBKEYHASH> {
    
    CKeyID hash;
    
    ScriptOutput(blocksci::uint160 &pubkeyHash) : hash{pubkeyHash} {}
    
    void processOutput(AddressState &, AddressWriter &writer);
    void checkOutput(const AddressState &, const AddressWriter &) {}
    
    bool isValid() const { return true; }
    
    blocksci::uint160 getHash() const;
};

template <>
struct ScriptOutput<blocksci::AddressType::Enum::WITNESS_PUBKEYHASH> {
    
    CKeyID hash;
    
    ScriptOutput(blocksci::uint160 &&pubkeyHash) : hash{pubkeyHash} {}
    
    void processOutput(AddressState &, AddressWriter &writer);
    void checkOutput(const AddressState &, const AddressWriter &) {}
    
    bool isValid() const { return true; }
    
    blocksci::uint160 getHash() const;
};

template <>
struct ScriptOutput<blocksci::AddressType::Enum::SCRIPTHASH> {
    CKeyID hash;
    
    ScriptOutput(blocksci::uint160 hash_) : hash(hash_) {}
    
    void processOutput(AddressState &, AddressWriter &writer);
    void checkOutput(const AddressState &, const AddressWriter &) {}
    
    blocksci::uint160 getHash() const;
    
    bool isValid() const {
        return true;
    }
};

template <>
struct ScriptOutput<blocksci::AddressType::Enum::WITNESS_SCRIPTHASH> {
    blocksci::uint256 hash;
    
    ScriptOutput(blocksci::uint256 hash_) : hash(hash_) {}
    
    void processOutput(AddressState &, AddressWriter &writer);
    void checkOutput(const AddressState &, const AddressWriter &) {}
    
    blocksci::uint160 getHash() const;
    
    bool isValid() const {
        return true;
    }
};

template <>
struct ScriptOutput<blocksci::AddressType::Enum::MULTISIG> {
    static constexpr int MAX_ADDRESSES = 16;
    using RawAddressArray = std::vector<CPubKey>;
    using ProcessedAddressArray = std::vector<uint32_t>;
    uint8_t numRequired;
    uint8_t numTotal;
    uint16_t addressCount;
    
    ProcessedAddressArray processedAddresses;
    RawAddressArray addresses;
    
    ScriptOutput() : addressCount(0) {}
    
    void addAddress(const std::vector<unsigned char> &vch1);
    
    bool isValid() const {
        return numRequired <= numTotal && numTotal == addressCount;
    }
    
    blocksci::uint160 getHash() const;
    void processOutput(AddressState &state, AddressWriter &writer);
    void checkOutput(const AddressState &state, const AddressWriter &writer);
};

template <>
struct ScriptOutput<blocksci::AddressType::Enum::NONSTANDARD> {
    CScript script;
    
    ScriptOutput<blocksci::AddressType::Enum::NONSTANDARD>() {}
    ScriptOutput<blocksci::AddressType::Enum::NONSTANDARD>(const CScript &script);
    
    void processOutput(AddressState &, AddressWriter &writer);
    void checkOutput(const AddressState &, const AddressWriter &) {}
    
    bool isValid() const {
        return true;
    }
};

template <>
struct ScriptOutput<blocksci::AddressType::Enum::NULL_DATA> {
    std::vector<unsigned char> fullData;
    
    ScriptOutput<blocksci::AddressType::Enum::NULL_DATA>(const CScript &script);
    
    void processOutput(AddressState &, AddressWriter &writer);
    void checkOutput(const AddressState &, const AddressWriter &) {}
    
    bool isValid() const {
        return true;
    }
};

using ScriptOutputType = blocksci::to_script_variant_t<ScriptOutput, blocksci::AddressInfoList>;

template <blocksci::AddressType::Enum type>
std::pair<blocksci::Address, bool> getAddressNum(ScriptOutput<type> &data, AddressState &state);

template <blocksci::AddressType::Enum type>
std::pair<blocksci::Address, bool> checkAddressNum(ScriptOutput<type> &data, const AddressState &state);

blocksci::AddressType::Enum addressType(const ScriptOutputType &type);

blocksci::Address processOutput(ScriptOutputType &scriptOutput, AddressState &state, AddressWriter &addressWriter);
blocksci::Address checkOutput(ScriptOutputType &scriptOutput, const AddressState &state, const AddressWriter &addressWriter);

ScriptOutputType extractScriptData(const unsigned char *scriptBegin, const unsigned char *scriptEnd, bool witnessActivated);

#endif /* script_output_hpp */
