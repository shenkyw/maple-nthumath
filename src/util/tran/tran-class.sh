#!/bin/bash
########�q
#                 (0)�ȯS�Դ���           ����/�t��
#                 (1)�߸̦���             �Z�O/�t��
#                 (2)�Ƥj�ۭ�             �t��/�ƾ�
#                 (3)�����G�m             �綤
#                 (4)������k             �ӤH/����
#                 (5)����M��             �ն�/����
#                 (6)�߱��I�w             �ͽ�/���P
#                 (7)�T�֥���             ���/����/�B��/��
#                 (8)��T�n��             �q��/�{��
#                 (9)�ͬ��s��             �ͬ�/�s��
########��
#     1  �� BBS/         ���� �� �ТТ�����ư�
#     2  �� Classes/     ���� �� �Z�O�t��
#     3  �� Personal/    ���� �� �ʮa����
#     4  �� Group/       ���� �� ����/�t��
#     5  �� Camp/        ���� �� �綤
#     6  �� School/      ���� �� ����M��
#     7  �� Feeling/     ���� �� �߱��d��
#     8  �� Math/        ���� �� �ƾ�
#     9  �� Computer/    ���� �� ��T�n��
#    10  �� Life/        ���� �� �ͬ��s��
#    11  �� Test/        ���� �� ���ȹ����
#    12  �� Else/        ���� �� �T�֥���
cd ~bbs/src/util/tran
./brd2gem ���� BBS
./brd2gem �t�� BBS
./brd2gem �Z�O Classes
./brd2gem �t�� Classes
./brd2gem �t�� Group
./brd2gem �ƾ� Math
./brd2gem �綤 Camp
./brd2gem �ӤH Personal
./brd2gem ���� Group
./brd2gem �ն� School
./brd2gem ���� School
./brd2gem �ͽ� Feeling
./brd2gem ���P Feeling
./brd2gem ��� Else
./brd2gem ���� Else
./brd2gem �B�� Else
./brd2gem �� Else
./brd2gem �q�� Computer
./brd2gem �{�� Computer
./brd2gem �ͬ� Life
./brd2gem �s�� Life
