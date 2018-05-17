from Queryable import Queryable


def fibo():
    ppf, pf = 1, 0
    while True:
        f = ppf + pf
        ppf, pf = pf, f
        print(str(f))
        yield f


def readlines(filename):
    with open(filename, "r") as f:
        for line in f:
            yield line.rstrip("\n")


def main():
    print(
       Queryable(range(10)).Where(lambda x: x % 2 == 0)
                           .Select(lambda x: x ** 2)
                           .Take(2)
                           .ToLost()
    )

    print(
        Queryable(fibo()).Where(lambda x: x % 3 == 0)
                         .Select(lambda x: x ** 2 if x % 2 == 0 else x)
                         .Take(5)
                         .ToLost()
    )

    print(
        Queryable(readlines("input.txt")).Select(lambda x: x.split(" "))
                                         .Flatten()
                                         .GroupBy(lambda x: x)
                                         .Select(lambda x: (x[0], len(x[1])))
                                         .OrderBy(lambda x: -x[1])
                                         .ToLost()
    )


if __name__ == "__main__":
    main()
