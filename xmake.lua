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
    add_includedirs("src/error")
    add_includedirs("src/config")
    add_includedirs("include/stdc_implemented")

    set_optimize("faster")
    set_languages("c++17")
    add_cflags("-static")
    add_cflags("-lm")