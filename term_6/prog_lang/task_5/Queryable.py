from collections import defaultdict


class Queryable:

    def __init__(self, sequence):
        self.uniterable = (tuple, dict, str)
        self.sequence = sequence


    def Select(self, func):
        sequence = self.sequence

        def select():
            for x in sequence:
                yield func(x)

        self.sequence = select()
        return self


    def Flatten(self):

        def flatten(sequence):
            if isinstance(sequence, self.uniterable):
                yield sequence
            else:
                try:
                    for value in sequence:
                        yield from flatten(value)
                except TypeError:
                    yield sequence

        self.sequence = flatten(self.sequence)
        return self


    def Where(self, func):
        sequence = self.sequence

        def where():
            for value in sequence:
                if func(value):
                    yield value

        self.sequence = where()
        return self


    def Take(self, k):
        sequence = self.sequence

        def take():
            for i in range(k):
                yield next(sequence)

        self.sequence = take()
        return self


    def GroupBy(self, key):
        sequence = defaultdict(lambda: [])
        for value in self.sequence:
            sequence[key(value)].append(value)
        self.sequence = iter(sequence.items())
        return self


    def OrderBy(self, key):
        self.sequence = iter(sorted(self.sequence, key=key))
        return self


    def ToList(self):
        seqlist = []
        for value in self.sequence:
            seqlist.append(value)
        return seqlist
