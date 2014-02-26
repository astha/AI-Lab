import re

#initialize stopWords
stopWords = []

#start replaceTwoOrMore
def replaceTwoOrMore(s):
    #look for 2 or more repetitions of character and replace with the character itself
    pattern = re.compile(r"(.)\1{1,}", re.DOTALL)
    return pattern.sub(r"\1\1", s)
#end

#start getStopWordList
def getStopWordList(stopWordListFileName):
    #read the stopwords file and build a list
    stopWords = []
    stopWords.append('AT_USER')
    stopWords.append('URL')

    fp = open(stopWordListFileName, 'r')
    line = fp.readline()
    while line:
        word = line.strip()
        stopWords.append(word)
        line = fp.readline()
    fp.close()
    return stopWords
#end

#start getfeatureVector
def getFeatureVector(tweet):
    featureVector = []
    #split tweet into words
    words = tweet.split()
    for w in words:
        #replace two or more with two occurrences
        w = replaceTwoOrMore(w)
        #strip punctuation
        w = w.strip('\'"?,.')
        #check if the word stats with an alphabet
        val = re.search(r"^[a-zA-Z][a-zA-Z0-9]*$", w)
        #ignore if it is a stop word
        if(w in stopWords or val is None):
            continue
        else:
            featureVector.append(w.lower())
    return featureVector
#end


#Read the tweets one by one and process it
fp = open('processedPositive', 'r')
line = fp.readline()
fp_out = open('featurePositive', 'w')

st = open('stopWords.txt', 'r')
stopWords = getStopWordList('stopWords.txt')

while line:
    processedTweet = line
    featureVector = getFeatureVector(processedTweet)
    fp_out.write(' '.join(featureVector))
    fp_out.write("\n")
    line = fp.readline()
#end loop
fp.close()
fp_out.close()


#Read the tweets one by one and process it
fp = open('processedNegative', 'r')
line = fp.readline()
fp_out = open('featureNegative', 'w')

while line:
    processedTweet = line
    featureVector = getFeatureVector(processedTweet)
    fp_out.write(' '.join(featureVector))
    fp_out.write("\n")
    line = fp.readline()
#end loop
fp.close()
fp_out.close()


#Read the tweets one by one and process it
fp = open('processedObjective', 'r')
line = fp.readline()
fp_out = open('featureObjective', 'w')

while line:
    processedTweet = line
    featureVector = getFeatureVector(processedTweet)
    fp_out.write(' '.join(featureVector))
    fp_out.write("\n")
    line = fp.readline()
#end loop
fp.close()
fp_out.close()