# -*- coding: utf-8 -*-
import numpy
import os

from matplotlib import pyplot

number = 100 # number of words to export
store_matrix = True # export watrix as image
starting_letter = None # starting letter

BASE_DIR = os.path.dirname(os.path.abspath(__file__))

file_path = os.path.join(BASE_DIR, "..", "data", "words.txt")


def main():
    with open(file_path, "r") as f:
        matrix = compute_matrix(f)

    if store_matrix:
        save_matrix(matrix, "matrix")

    generate_words(
        matrix,
        word_numbers=number,
        size_min=4,
        size_max=14,
        start=starting_letter
    )

def compute_matrix(words):
    #make a matrix of all possible int
    matrix = numpy.zeros((256, 256, 256), dtype='int32')
    for word in words:
        word = word.strip().lower()
        i, j = 0, 0
        for k in [ord(c) for c in list("%s\n" % word)]: #\n for ending
            if k >= 256:
                continue
            #add 1 to sectition of i,j 
            matrix[i, j, k] += 1
            #incrementation
            i, j = j, k
    return matrix

def save_matrix(matrix, filename):
    count2D=matrix.sum(axis=0)
    p2D=count2D.astype('float')/numpy.tile(sum(count2D.T), (256, 1)).T
    p2D[numpy.isnan(p2D)] = 0

    # For better contrast, we plot p^alpha instead of p
    alpha = 0.33
    p2Da = p2D**alpha

    # We display only letters a to z, ie ASCII from 97 to 123.
    a = ord('a')
    z = ord('z')
    pyplot.figure(figsize=(8, 8))
    pyplot.imshow(p2Da[a:z+1, a:z+1], interpolation='nearest')
    pyplot.axis('off')

    for i in range(a, z +1):
        pyplot.text(-1, i-a, chr(i), horizontalalignment='center', verticalalignment='center')
        pyplot.text(i-a, -1, chr(i), horizontalalignment='center', verticalalignment='center')
    pyplot.savefig(filename + ".png")


def generate_words(matrix, word_numbers=100, size_min=4, size_max=14, start=None):
    numpy.random.seed(None)
    s = matrix.sum(axis=2)
    st = numpy.tile(s.T, (256, 1, 1)).T
    p = matrix.astype('float')/st
    p[numpy.isnan(p)] = 0

    total = 0
    while total < word_numbers:
        if start:
            i, j = 0, ord(start[0])
            res = start
        else:
            i,j = 0,0
            res = ''

        while not j==ord('\n'):

            #avoid non ascii char
            k = 255
            retry = 10
            while k > 128:
                k = numpy.random.choice(list(range(256)), 1 ,p=p[i,j,:])[0]
                retry -= 1
                if retry == 0: #avoid infinit loops
                    k = ord('\n')

            res = res + chr(k)
            i, j = j, k

        res = res[:-1] #remove trailling \n
        if len(res) >= size_min and len(res) <= size_max:
            total += 1
            print(res) 


if __name__ == "__main__":
    main()

