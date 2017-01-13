// This file is a part of the IncludeOS unikernel - www.includeos.org
//
// Copyright 2016-2017 Oslo and Akershus University College of Applied Sciences
// and Alfred Bratterud
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//     http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.

#include <common.cxx>
#include <memdisk>

CASE("memdisk properties")
{
  auto disk = fs::new_shared_memdisk();
  EXPECT(disk->empty() == true);
  EXPECT(disk->device_id() == 0);
  EXPECT(disk->fs_ready() == false);
  EXPECT(disk->name() == "memdisk0");
  EXPECT(disk->dev().size() == 0ull);
  EXPECT(disk->dev().device_type() == "Block device");
  EXPECT(disk->dev().driver_name() == "MemDisk");
  bool enumerated_partitions {false};
  auto part_fn = [&enumerated_partitions, lest_env](fs::error_t err, std::vector<fs::Disk::Partition>& partitions) {
    if (!err) {
      enumerated_partitions = true;
      EXPECT(partitions.size() == 4u); // 4 is default number
    }
  };
  disk->partitions(part_fn);
  EXPECT(enumerated_partitions == true);
}
