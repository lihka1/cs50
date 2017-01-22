import nltk

class Analyzer():
    """Implements sentiment analysis."""

    def __init__(self, positives, negatives):
        """Initialize Analyzer."""
        self.positives = set()
        self.negatives = set()
        with open(positives) as lines:
            for line in lines:
                if not line.startswith(';'):
                    clean = line.strip()
                    if clean:
                        self.positives.add(clean.lower())
        with open(negatives) as lines:
            for line in lines:
                if not line.startswith(';'):
                    clean = line.strip()
                    if clean:
                        self.negatives.add(clean.lower())
                        
        # TODO

    def analyze(self, text):
        """Analyze text for sentiment, returning its score."""

        # TODO
        tokenizer = nltk.tokenize.TweetTokenizer()
        tokens = tokenizer.tokenize(text)
        tot = 0
        for token in tokens:
            if token.lower() in self.positives:
                tot += 1
            elif token.lower() in self.negatives:
                tot -= 1
        return tot
