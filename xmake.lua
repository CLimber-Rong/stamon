--[[
Name: xmake.lua
License: Apache 2.0
Author: CLimber-Rong
Date: 03/11/24 19:15
Description: Stamon的XMake构建文件
--]]
target("stamon")
    set_kind("binary")

    add_files("src/Main.cpp")

    add_includedirs("src")
    add_includedirs("src/ast")
    add_includedirs("src/data_type")
    add_includedirs("src/vm")
    add_includedirs("src/ir")
    add_includedirs("src/compiler")
    add_includedirs("src/sfn")
    add_includedirs("src/action")
    add_includedirs("src/exception")
    add_includedirs("src/config")

    add_includedirs("include/pure")
    add_includedirs("include/ArrayList/interface")
    add_includedirs("include/BasicIo/interface")
    add_includedirs("include/BasicPlatform/interface")
    add_includedirs("include/EasySmartPtr/interface")
    add_includedirs("include/FileReader/interface")
    add_includedirs("include/FileWriter/interface")
    add_includedirs("include/HashMap/interface")
    add_includedirs("include/MemoryPool/interface")
    add_includedirs("include/Stack/interface")
    add_includedirs("include/String/interface")

    add_includedirs("include/ArrayList/stdcpp")
    add_includedirs("include/BasicIo/stdcpp")
    add_includedirs("include/BasicPlatform/stdcpp")
    add_includedirs("include/EasySmartPtr/stdcpp")
    add_includedirs("include/FileReader/stdcpp")
    add_includedirs("include/FileWriter/stdcpp")
    add_includedirs("include/HashMap/stdcpp")
    add_includedirs("include/MemoryPool/stdc")
    add_includedirs("include/Stack/stdcpp")
    add_includedirs("include/String/stdcpp")

    set_optimize("faster")
    set_languages("c++17")
    add_cflags("-static")
    add_cflags("-lm")