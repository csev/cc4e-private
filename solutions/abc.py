
def words(document):
    collection = []
    for line in document:
        for word in line.split():
            if word not in collection :
                collection.append(word)
    return collection

document = ["this is a test", "a test is cool"]
print(words(document))
