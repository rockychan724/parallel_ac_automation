# -*- encoding: utf-8 -*-

import random


def get_words_from_wiki(total, min_len, max_len):
    with open("wiki_en_1.txt", 'r', encoding="utf-8") as fr, open("./words.txt", 'w', encoding="utf-8") as fw:
        words = []
        for line in fr.readlines():
            words += line.strip().split(' ')
        print(len(words))
        words = list(set(words))  # 去除重复
        print(len(words))
        random.shuffle(words)
        i = 0
        for w in words:
            if min_len <= len(w) <= max_len:
                i += 1
                fw.write(w + '\n')
                if i >= total:
                    break


if __name__ == '__main__':
    get_words_from_wiki(20, 2, 10)
