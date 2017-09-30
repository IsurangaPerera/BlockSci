//
//  data_access.hpp
//  BlockReader
//
//  Created by Harry Kalodner on 1/18/17.
//  Copyright © 2017 Harry Kalodner. All rights reserved.
//

#ifndef data_access_hpp
#define data_access_hpp

#include <blocksci/data_configuration.hpp>

namespace blocksci {
    
    class ChainAccess;
    class ScriptAccess;
    class ScriptFirstSeenAccess;
    class AddressIndex;
    class HashIndex;

    class DataAccess {
    public:
        const DataConfiguration config;
        std::unique_ptr<ChainAccess> chain;
        std::unique_ptr<ScriptAccess> scripts;
        std::unique_ptr<ScriptFirstSeenAccess> scriptFirstSeen;
        std::unique_ptr<AddressIndex> addressIndex;
        std::unique_ptr<HashIndex> hashIndex;
        
        static DataAccess &Instance(const DataConfiguration &config, bool errorOnReorg, uint32_t blocksIgnored);
        static DataAccess &Instance();
        
        DataAccess(const DataConfiguration &config, bool errorOnReorg, uint32_t blocksIgnored);
        
        // delete copy and move constructors and assign operators
        DataAccess(DataAccess const&) = delete;             // Copy construct
        DataAccess(DataAccess&&) = delete;                  // Move construct
        DataAccess& operator=(DataAccess const&) = delete;  // Copy assign
        DataAccess& operator=(DataAccess &&) = delete;      // Move assign
        
    };
}

#endif /* data_access_hpp */
