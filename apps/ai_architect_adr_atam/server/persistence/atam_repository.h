#pragma once

#include <optional>
#include <string>
#include <vector>

#include "domain/atam.h"
#include "persistence/file_store.h"

namespace adra::persistence {

class AtamRepository {
public:
    explicit AtamRepository(FileStore& store);

    bool save(const domain::AtamSession& session);
    std::optional<domain::AtamSession> find(const std::string& id);
    std::vector<domain::AtamSession> list();
    bool remove(const std::string& id);

private:
    std::string filename_for_id(const std::string& id) const;

    FileStore& store_;
};

}  // namespace adra::persistence
