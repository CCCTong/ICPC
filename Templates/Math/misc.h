#include <bits/stdc++.h>

//  Cantor expansion
ll cexp(array<int, S> a) {
	int res = 0;
	for (int i = 0; i != S - 1; ++i)
		for (int j = i + 1; j != S; ++j)
			if (a[i] > a[j]) res += f[S - 1 - i];
	return res;
}

array<int, S> icexp(ll x) {
	array<int, S> res;
	vector<int> v(S); iota(v.begin(), v.end(), 0);
	for (int i = 0; i != S; ++i) {
		int t = f[S - i - 1];
		res[i] = v[x / t];
		v.erase(v.begin() + x / t);
		x %= t;
	}
	return res;
}

struct frac {
    ll n, d;
    frac(ll nn = 0, ll dd = 1) { ll w = gcd(nn, dd); n = nn / w; d = dd / w;}
    frac operator-() const { return frac(-n, d); }
    frac operator+(frac f) { return frac(n * f.d + d * f.n, d * f.d); }
    frac operator-(frac f) { return frac(n * f.d - d * f.n, d * f.d); }
    frac operator*(frac f) { return frac(n * f.n, d * f.d); }
    frac operator/(frac f) { return frac(n * f.d, d * f.n); }
};

//	Solve cubic/quadratic equation
#define sq(x) ((x) * (x))
#define cb(x) (sq(x) * (x))
typedef complex<dbl> cplx;
void cubic(dbl a0, dbl a1, dbl a2, dbl a3,
	cplx& x1, cplx& x2, cplx& x3) {
	a0 /= a3; a1 /= a3; a2 /= a3;
	dbl p = sq(a2) / 9 - a1 / 3, q = a2 * a1 / 6 - cb(a2) / 27 - a0/ 2;
	cplx del = sqrt((cplx)(sq(q) - cb(p))), w(-0.5, sqrt(3) / 2),
		f1 = pow(q + del, 1. / 3), f2 = pow(q - del, 1. / 3);
	x1 = f1 + f2 - a2 / 3;
	x2 = w * f1 + sq(w) * f2 - a2 / 3;
	x3 = sq(w) * f1 + w * f2 - a2 / 3;
}

void quadr(dbl a0, dbl a1, dbl a2, dbl a3, dbl a4,
	cplx& x1, cplx& x2, cplx& x3, cplx& x4) {
	a0 /= a4; a1 /= a4; a2 /= a4; a3 /= a4; a3 /= 4;
	dbl b = a2 - 6 * sq(a3), c = 3 * (a1 - 2 * a2 * a3 + cb(2 * a3)),
		d = 12 * (a0 - a1 * a3 + a2 * sq(a3) - 3 * sq(sq(a3))),
		p = sq(b) + d, q = 2 * cb(b) + 3 * sq(c) - 6 * b * d, k = 2 * sqrt(3);
	cplx del = sqrt((cplx(sq(q) - 4 * cb(p)))),
		f = pow((q + del) / 2., 1. / 3) + pow((q - del) / 2., 1. / 3),
		f1 = sqrt(f - 2 * b), f2 = k * c / f1, f3 = -4 * b - f;
	x1 = (f1 - sqrt(f3 - f2)) / k - a3; x2 = (f1 + sqrt(f3 - f2)) / k - a3;
	x3 = (-f1 - sqrt(f3 + f2)) / k - a3; x4 = (-f1 + sqrt(f3 + f2)) / k - a3;
}

//	Simpson's Rule
template<class F> dbl sps0(const F& f,dbl l, dbl r) {
	return (f(l) + 4 * f((l + r) / 2) + f(r)) * (r - l) / 6;
}

template<class F> dbl sps1(const F& f, dbl l, dbl r, dbl eps, dbl s) {
	dbl m = (l + r) / 2, lv = sps0(f, l, m), rv = sps0(f, m, r), d = lv + rv - s;
	if (fabs(d) < 15 * eps) return lv + rv + 15 * d;
	else return sps1(f, l, m, eps / 2, lv) + sps1(f, m, r, eps / 2, rv);
}

template<class F> dbl spsint(const F& f, dbl l, dbl r, dbl eps) {
	return sps1(f, l, r, eps, sps0(f, l, r));
}

//	FFT/NTT/FWT
ui fr[N], fs;

void init(ui s) {
	for (fs = 1; fs <= s; fs <<= 1);
	for (int i = 0; i != fs; ++i)
		fr[i] = (fr[i >> 1] >> 1) | (i & 1 ? (fs >> 1) : 0);
}


void fft(cplx* p, int f) {
	for (int i = 0; i != fs; ++i) if (i < fr[i]) swap(p[i], p[fr[i]]);
	for (int i = 1; i != fs; i <<= 1) {
		cplx w0{ cos(pi / i), (f ? 1. : -1.) * sin(pi / i) };
		for (int j = 0; j != fs; j += (i << 1)) {
			cplx w{ 1, 0 };
			for (int k = 0; k != i; k++, w = w * w0) {
				cplx u = p[j + k], v = w * p[i + j + k];
				p[j + k] = u + v; p[i + j + k] = u - v;
			}
		}
	}
}

void fft_res(cplx* p) {
	for (ui i = 0; i != fs; ++i)
		p[i] = p[i] * (1. / fs);
}


void ntt(ul* p, int f) {
	static const ul g;
	// ((3u<<30)|1)->5; ((119u<<23)|1)->3; ((479u<<21)|1)->3;
	for (int i = 0; i != fs; ++i) if (i < fr[i]) swap(p[i], p[fr[i]]);
	for (int i = 1; i != fs; i <<= 1) {
		ul e = (MOD - 1) / (i << 1), w0 = qp(g, f ? e : MOD - 1 - e);
		for (int j = 0; j != fs; j += (i << 1)) {
			ul w = 1;
			for (int k = 0; k != i; k++, w = M(w * w0)) {
				ul u = p[j + k], v = M(w * p[i + j + k]);
				p[j + k] = M(u + v); p[i + j + k] = M(u - v);
			}
		}
	}
}

void ntt_res(ul* p) {
	for (ul i = 0, is = inv(fs); i != fs; ++i)
		p[i] = M(p[i] * is);
}

void fwt(ui* a, ui n) {
	for (ui k = 1; k < n; k <<= 1)
		for (ui m = k << 1, i = 0; i < n; i += m)
			for (ui j = 0; j != k; j++) {
				ui x = a[i + j], y = a[i + j + k];
				//^: a[i + j] = x + y, a[i + j + d]= x - y;
				//&: a[i + j] = x + y;
				//|: a[i + j + d] = x + y;
			}
}


void ifwt(ui* a, ui n) {
	for (ui k = 1; k < n; k <<= 1)
		for (ui m = k << 1, i = 0; i < n; i += m)
			for (ui j = 0; j != k; j++) {
				ui x = a[i + j], y = a[i + j + k];
				//^: a[i + j] = (x + y) / 2, a[i + j + d] = (x - y) /2;
				//&: a[i + j] = x - y;
				//|: a[i + j + d] = y - x;
			}
}
