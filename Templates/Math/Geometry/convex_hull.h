#include "basic_geometry_2d.h"

int convex_hull(vec* pv, int n, vec* cv) {
    sort(pv + 1, pv + n + 1);
    int m = 0;
    for (int i = 1; i <= n; ++i) {
        while (m>1 && dc(crx(cv[m-1], cv[m], pv[i]))==-1) m--;
        cv[++m] = pv[i];
    }
    int k = m;
    for (int i = n - 1; i; --i) {
        while (m>k && dc(crx(cv[m-1], cv[m], pv[i]))==-1) m--;
        cv[++m] = pv[i];
    }
    return m>1?m-1:m;
}

bool judge(line l0, line l1, line l2) { return dc((litsc(l1, l2)-l0.p)^l0.v)==1; }
int halfplane_intersection(line* lv, int n, vec* pv) {
    static pair<pair<dbl,dbl>, int> a[N];
    for (int i = 1; i <= n; ++i)
        a[i] = { { arg(lv[i].v), lv[i].p*univ(arg(lv[i].v)-pi/2) }, i };
    sort(a + 1, a + n + 1);
    static int b[N], q[N]; int w = 0, l = 1, r = 0;
    for (int i = 1; i <= n; ++i)
        if (i == 1 || dc(a[i].first.first-a[i-1].first.first))
            b[++w] = a[i].second;
    for (int i = 1; i <= w; ++i) {
        while (l<r&&judge(lv[b[i]],lv[q[r]],lv[q[r-1]]))--r;
        while (l<r&&judge(lv[b[i]],lv[q[l]],lv[q[l+1]]))++l;
        q[++r]=b[i];
    }
    while(l<r&&judge(lv[q[l]],lv[q[r]],lv[q[r-1]]))--r;
    while(l<r&&judge(lv[q[r]],lv[q[l]],lv[q[l+1]]))++l;
    if (r <= l + 1) return 0;
    int m = 0; q[r+1]=q[l];
    for (int i = l; i <= r; ++i)
        pv[++m]=litsc(lv[q[i]],lv[q[i+1]]);
    return m;
}

int minkowski_sum(vec* cv1, int n1, vec* cv2, int n2, vec* cv) {
    static vec dv1[N], dv2[N], dv;
    for (int i = 1; i <= n1; ++i) dv1[i] = cv1[i + 1] - cv1[i];
    for (int i = 1; i <= n2; ++i) dv2[i] = cv2[i + 1] - cv2[i];
    int m = 0, p = 1, q = 1; cv[++m] = cv1[1] + cv2[1];
    while (p<=n1 || q<=n2) {
        if (p<=n1&&q<=n2) dv=dc(dv1[p]^dv2[q])==-1?dv1[p++]:dv2[q++];
        else if(p<=n1) dv=dv1[p++]; else dv=dv2[q++];
        while (m>1&&!dc((cv[m]-cv[m-1])^dv)) --m;
        cv[m+1]=cv[m]+dv;m++;
    }
    return m>1?m-1:m;
}