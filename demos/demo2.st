import std;

func quicksort(s, l, r) {
    if l<r {
        def i = l, j = r;
        def x = s[l];
        while i<j {
            while i<j && s[j]>=x {
                j-=1;
            }
            if i<j {
                s[i] = s[j];
                i += 1;
            }
            while i<j && s[i]<x {
                i+=1;
            }
            if i<j {
                s[j] = s[i];
                j -= 1;
            }
        }
        s[i] = x;

        quicksort(s, l, i-1);
        quicksort(s, i+1, r);
    }
}

def main = func {
    def arr = {4, 5, 1, 4, 1};
    quicksort(arr, 0, len(arr)-1);
    println(arr);
    return 0;
}();