#import regex
import re

#start process_tweet
def processTweet(tweet):
    # process the tweets

    #Convert to lower case
    tweet = tweet.lower()

    # remove the subject
    # tweet = re.sub('^.*\t','',tweet)

    #Convert www.* or https?://* to URL
    tweet = re.sub('((www\.[\s]+)|(https?://[^\s]+))','URL',tweet)
    #Convert @username to AT_USER
    tweet = re.sub('@[^\s]+','AT_USER',tweet)
    #Remove additional white spaces
    tweet = re.sub('[\s]+', ' ', tweet)
    #Replace #word with word
    tweet = re.sub(r'#([^\s]+)', r'\1', tweet)
    #trim
    tweet = tweet.strip('\'"')
    return tweet
#end

#Read the tweets one by one and process it
fp = open('check_tweets', 'r')
fp_out = open('processedTest', 'w')
line = fp.readline()

while line:
    processedTweet = processTweet(line)
    fp_out.write(processedTweet)
    fp_out.write("\n")
    line = fp.readline()
#end loop

fp_out.close()
fp.close()

# #Read the tweets one by one and process it
# fp = open('twitter_negative', 'r')
# fp_out = open('processedNegative', 'w')
# line = fp.readline()

# while line:
#     processedTweet = processTweet(line)
#     fp_out.write(processedTweet)
#     fp_out.write("\n")
#     line = fp.readline()
# #end loop
# fp_out.close()
# fp.close()


# #Read the tweets one by one and process it
# fp = open('twitter_objective', 'r')
# fp_out = open('processedObjective', 'w')
# line = fp.readline()

# while line:
#     processedTweet = processTweet(line)
#     fp_out.write(processedTweet)
#     fp_out.write("\n")
#     line = fp.readline()
# #end loop
# fp_out.close()
# fp.close()