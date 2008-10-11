#!/bin/bash
########從
#                 (0)亞特拉斯城           站務/系統
#                 (1)心裡有數             班板/系務
#                 (2)數大招風             系隊/數學
#                 (3)風的故鄉             營隊
#                 (4)眾望所歸             個人/團體
#                 (5)水木清華             校園/社團
#                 (6)心情點滴             友誼/情感
#                 (7)娛樂市場             文學/音樂/運動/休閒
#                 (8)資訊軟體             電腦/程式
#                 (9)生活廣場             生活/新竹
########到
#     1  ★ BBS/         分類 □ ＢＢＳ相關事務
#     2  ★ Classes/     分類 □ 班板系務
#     3  ★ Personal/    分類 □ 百家爭鳴
#     4  ★ Group/       分類 □ 團體/系隊
#     5  ★ Camp/        分類 □ 營隊
#     6  ★ School/      分類 □ 水木清華
#     7  ★ Feeling/     分類 □ 心情留言
#     8  ★ Math/        分類 □ 數學
#     9  ★ Computer/    分類 □ 資訊軟體
#    10  ★ Life/        分類 □ 生活廣場
#    11  ★ Test/        分類 □ 站務實驗區
#    12  ★ Else/        分類 □ 娛樂市場
cd ~bbs/src/util/tran
./brd2gem 站務 BBS
./brd2gem 系統 BBS
./brd2gem 班板 Classes
./brd2gem 系務 Classes
./brd2gem 系隊 Group
./brd2gem 數學 Math
./brd2gem 營隊 Camp
./brd2gem 個人 Personal
./brd2gem 團體 Group
./brd2gem 校園 School
./brd2gem 社團 School
./brd2gem 友誼 Feeling
./brd2gem 情感 Feeling
./brd2gem 文學 Else
./brd2gem 音樂 Else
./brd2gem 運動 Else
./brd2gem 休閒 Else
./brd2gem 電腦 Computer
./brd2gem 程式 Computer
./brd2gem 生活 Life
./brd2gem 新竹 Life
