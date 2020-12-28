from typing import List

def quicksort(pred, lst: List[int]) -> List[int]:
    if len(lst) == 0:
        return []
    else:
        pivot = lst[0]
        less  = list(filter(lambda x: pred(x, pivot), lst[1:]))
        great = list(filter(lambda x: not pred(x, pivot), lst[1:]))

        return quicksort(pred, less) + [pivot] + quicksort(pred, great)


test = [-87, -100, -8, -91, -25, -42, -35, 50, 0, -33, -43, 62, -8, -52, -92,
        -20, 1, -66, -46, 94, -15, -17, 33, 43, 17, -16, 63, -9, 23, -70, -94,
        51, 55, -71, 86, 0, -54, -50, -74, 85, 27, 86, 30, -48, 69, 4, 80, -33,
        3, 92, 6, -65, -79, 7, -11, 24, -30, -48, 87, 52, -57, -72, 0, 79, 14,
        5, -41, -37, -50, 1, -62, 32, -45, 24, 57, 55, -80, 74, 38, 56, 14, 2,
        -7, -36, -100, -49, 96, -68, -97, -61, 35, -26, 75, 100, -79, -12, -7,
        2, 50, 72, -70, -94, 51, 55, -71, 86, 0, -54, -50, -74, 85, 27, 86, 30]
        
print(quicksort(lambda x,y: x < y, test))
