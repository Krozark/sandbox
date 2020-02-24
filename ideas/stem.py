from nltk.stem.snowball import SnowballStemmer
stemmer = SnowballStemmer("french")
print(stemmer.stem("courir"))
