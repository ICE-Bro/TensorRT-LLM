/*
 * Copyright (c) 2022-2024, NVIDIA CORPORATION.  All rights reserved.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */
#pragma once

#if ENABLE_MULTI_DEVICE

#include <cstdint>
#include <memory>

namespace tensorrt_llm::runtime
{
////////////////////////////////////////////////////////////////
// Functions for sharing file descriptors between processes
////////////////////////////////////////////////////////////////
struct NcclIpcSocket;

std::shared_ptr<NcclIpcSocket> ncclIpcSocketInit(int rank, uint64_t hash, uint32_t volatile* abortFlag);

void ncclIpcSocketClose(std::shared_ptr<NcclIpcSocket> handle);

int ncclIpcSocketRecvFd(std::shared_ptr<NcclIpcSocket> handle);

void ncclIpcSocketSendFd(std::shared_ptr<NcclIpcSocket> handle, int fd, int rank, uint64_t hash);

} // namespace tensorrt_llm::runtime

#endif // ENABLE_MULTI_DEVICE
