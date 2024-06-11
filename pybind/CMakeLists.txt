# Copyright (c) 2024 General Motors GTO LLC
#
# Licensed to the Apache Software Foundation (ASF) under one
# or more contributor license agreements.  See the NOTICE file
# distributed with this work for additional information
# regarding copyright ownership.  The ASF licenses this file
# to you under the Apache License, Version 2.0 (the
# "License"); you may not use this file except in compliance
# with the License.  You may obtain a copy of the License at
#
#   http://www.apache.org/licenses/LICENSE-2.0
#
# Unless required by applicable law or agreed to in writing,
# software distributed under the License is distributed on an
# "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY
# KIND, either express or implied.  See the License for the
# specific language governing permissions and limitations
# under the License.
# cmake_minimum_required(VERSION 3.16)
# project(up-abs-py VERSION 0.1.0 LANGUAGES CXX)
# set(CMAKE_CXX_STANDARD 17)

# find_package(nlohmann_json REQUIRED)

set(PYBIND11_FINDPYTHON True)
find_package(pybind11 CONFIG)

pybind11_add_module(pyUpAbsTrans src/module_main.cpp)
target_link_libraries(pyUpAbsTrans PUBLIC UpAbstractTransport nlohmann_json::nlohmann_json)


# INSTALL(TARGETS ${PROJECT_NAME})
# INSTALL(DIRECTORY include DESTINATION .)
