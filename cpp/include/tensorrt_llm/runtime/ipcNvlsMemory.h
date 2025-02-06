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

#include "tensorrt_llm/common/mpiUtils.h"
#include "tensorrt_llm/runtime/worldConfig.h"

#include <algorithm>
#include <cuda.h>
#include <cuda_runtime.h>
#include <set>

namespace tensorrt_llm::runtime
{
struct IpcNvlsHandle
{
    size_t size = 0;
    uintptr_t uc_ptr = 0;
    uintptr_t mc_ptr = 0;
    CUdeviceptr uc_va;
    CUdeviceptr mc_va;
    CUmemGenericAllocationHandle uc_handle;
    CUmemGenericAllocationHandle mc_handle;
};

void MPI_group_barrier(std::set<int> ranks);

bool ipcNvlsSupported();

IpcNvlsHandle ipcNvlsAllocate(size_t size, std::set<int> ranks);

void ipcNvlsFree(IpcNvlsHandle handle);

template <typename T>
class DeviceAllocationNvls
{
public:
    DeviceAllocationNvls() = default;

    ~DeviceAllocationNvls()
    {
        this->free();
    }

    void reset(size_t size, std::set<int> ranks)
    {
        this->free();
        _handle = ipcNvlsAllocate(size * sizeof(T), ranks);
        _capacity = size;
    }

    T* getMulticastPointer() const
    {
        return reinterpret_cast<T*>(_handle.mc_ptr);
    }

    T* getUnicastPointer() const
    {
        return reinterpret_cast<T*>(_handle.uc_ptr);
    }

    size_t getCapacity() const
    {
        return _capacity;
    }

    void free()
    {
        if (_capacity > 0)
        {
            ipcNvlsFree(_handle);
            _capacity = 0;
        }
    }

private:
    size_t _capacity = 0;
    IpcNvlsHandle _handle;
};
} // namespace tensorrt_llm::runtime
