#pragma once

#include <optional>
#include <string>
#include <vector>

#include "domain/adr.h"
#include "persistence/file_store.h"

namespace adra::persistence {

class AdrRepository {
public:
    explicit AdrRepository(FileStore& store);

    bool save(const domain::Adr& adr);
    std::optional<domain::Adr> find(const std::string& id);
    std::vector<domain::Adr> list();
    bool remove(const std::string& id);
    int next_number();

private:
    std::string filename_for(const domain::Adr& a) const;
    std::string filename_for_id(const std::string& id) const;

    FileStore& store_;
};

}  // namespace adra::persistence
