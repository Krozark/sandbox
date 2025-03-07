# pip install sentence-transformers, nltk
# Télécharger les ressources nécessaires pour NLTK
# nltk.download('punkt')
# nltk.download('stopwords')
# nltk.download('wordnet')

from sentence_transformers import SentenceTransformer, util
import string

from nltk.corpus import stopwords
from nltk.tokenize import word_tokenize
from nltk.stem import WordNetLemmatizer
import nltk



# Charger le modèle Sentence-BERT
model = SentenceTransformer('paraphrase-MiniLM-L6-v2')

# Définir les fonctions à appeler
def fonction_le_soleil_se_couche():
    print("Action : Le soleil se couche")

def fonction_le_jour_se_leve():
    print("Action : Le jour se lève")

# Dictionnaire de phrases et fonctions associées
phrases = {
    "le soleil se couche": fonction_le_soleil_se_couche,
    "le jour se lève": fonction_le_jour_se_leve
}

# Convertir les phrases en embeddings
phrase_embeddings = {phrase: model.encode(phrase) for phrase in phrases}

# Fonction pour vérifier la similarité sémantique
def check_similarity(input_text, threshold=0.7):
    input_embedding = model.encode(input_text)
    for phrase, embedding in phrase_embeddings.items():
        similarity = util.pytorch_cos_sim(input_embedding, embedding).item()
        if similarity > threshold:
            return phrases[phrase], similarity
    return None, 0

# Fonction pour détecter la fin d'une phrase
def is_end_of_sentence(word):
    return word[-1] in string.punctuation

# Fonction pour filtrer les tokens
def filter_tokens(word):
    # Supprimer les mots vides
    stop_words = set(stopwords.words('french'))
    if word.lower() in stop_words:
        return False
    # Supprimer les caractères spéciaux
    #if any(char.isdigit() or char in string.punctuation for char in word):
    # if any(char in string.punctuation for char in word):
    #     return False
    return True

# Paramètres de la fenêtre glissante
window_size = 10  # Taille de la fenêtre glissante

# Exemple d'utilisation
input_text = ""

# Initialiser le lemmatiseur
lemmatizer = WordNetLemmatizer()

text_example = (
    "Le soleil se couche lentement à l'horizon. "
    "La nuit tombe doucement sur la ville. "
    "Le jour se lève tôt le matin. "
    "Les oiseaux chantent joyeusement! "
    "Les enfants jouent dans le parc. "
    "Le ciel est bleu et clair aujourd'hui. "
    "Les nuages passent lentement. "
    "La pluie commence à tomber doucement. "
    "Les feuilles changent de couleur en automne. "
    "L'hiver approche rapidement!"
)

for word in word_tokenize(text_example, language='french'):
    # Filtrer les tokens
    if not filter_tokens(word):
        continue

     # Lemmatiser le mot
    lemmatized_word = lemmatizer.lemmatize(word.lower())
    print(f"{word} => {lemmatized_word}")

    # Ajouter le nouveau mot à input_text
    input_text += " " + lemmatized_word
    print(input_text)

    # Vérifier la similarité sémantique
    function, similarity = check_similarity(input_text)
    if function:
        print(f"Correspondance trouvée avec le text ’{input_text}’: ({similarity}) {function.__name__}")
        function()
        # Réinitialiser input_text après avoir trouvé une correspondance
        input_text = ""

    # Vérifier si le mot est une fin de phrase
    if is_end_of_sentence(word):
        # Réinitialiser input_text après une fin de phrase
        input_text = ""

    # Limiter la taille de input_text avec une fenêtre glissante
    if len(input_text.split()) > window_size:
        input_text = " ".join(input_text.split()[-window_size:])