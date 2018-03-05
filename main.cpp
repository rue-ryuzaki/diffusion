#include <cstdlib>
#include <iostream>
#include <vector>
#include <string>

uint dim;
uint dim_num;
uint leng;
uint ** delta;
uint * sum_point;
int ** start;

/*uint factorial (uint n) {
    return n<=1 ? 1 : n * factorial (n - 1);
}*/

uint power(uint base, uint n)
{
    uint result = 1;
    for (uint i = 0; i < n; ++i) {
        result *= base;
    }
    return result;
}

struct space
{
    space() : space (0, 0, 0) { }
    space(int p[3]) : space (p[0], p[1], p[2]) { }
    space(int x, int y, int z = 0) {
        point[0] = x;
        point[1] = y;
        point[2] = z;
    }
    
    std::vector<space> sp;
    int point[3];
};

void recursio(uint di, space& points, std::vector<uint>& parts, uint step)
{
    if (step >= parts.size()) {
        for (uint i = 0; i < dim; ++i) {
            uint d = abs(points.point[i] - start[di][i]);
            //cout << i << " start " << points.start[i] << " end " << points.point[i] << "  \tdelta=" << d << endl;
            delta[i][d] ++;
            ++sum_point[i];
        }
        return;
    }
    
    for (uint i = 0; i < dim; ++i) {
        space p(points.point);
        uint part = (parts[step] / power(2, i)) % 2;
        
        if (std::abs(p.point[i]) % 2 == part) {
            p.point[i] ++;
        } else {
            p.point[i] --;
        }
        points.sp.push_back(p);
        recursio(di, points.sp[i], parts, step + 1);
    }
    points.sp.clear();
}

void calculation(std::vector<uint>& parts)
{
    leng = 1 + parts.size();
    
    delta = new uint*[dim];
    sum_point = new uint[dim];
    
    for (uint i = 0; i < dim; ++i) {
        delta[i] = new uint[leng];
        sum_point[i] = 0;
        for (uint j = 0; j < leng; ++j) {
            delta[i][j] = 0;
        }
    }
    
    start = new int*[dim_num];
    
    for (uint i = 0; i < dim_num; ++i) {
        space point;
        start[i] = new int[dim];
        for (uint j = 0; j < dim; ++j) {
            point.point[j] = (i / power(2, j)) % 2;
            start[i][j] = (i / power(2, j)) % 2; // ?
        }
        recursio(i, point, parts, 0);
    }
    
    // print;
    /*for (uint i = 0; i < dim; ++i) {
        cout << endl;
        for (uint j = 0; j < leng + 1; ++j) {
            cout << delta[i][j] << endl;
        }
    }*/
    char axe[] = {'x', 'y', 'z'};
    double diffusio[dim];
    for (uint i = 0; i < dim; ++i) {
        diffusio[i] = 0;
        for (uint j = 0; j < leng; ++j) {
            //cout << i << " " << j<< " " << (double)delta[i][j] << endl;
            diffusio[i] += j * j * double(delta[i][j]) / sum_point[i];
        }
        diffusio[i] /= 2; // B -> D
        // print diffusio
        std::cout << "D(" << axe[i] << ")=\t" << diffusio[i] << std::endl;
    }
}

int main(int argc, char* argv[]) 
{
    if (argc != 3) {
        printf("Use key: [dimension] [array]\nexample diffusio 2 02130123");
        return 1;
    }
    try {
        dim = atoi(argv[1]);
    } catch (std::exception &e) {
        printf("Error read dimension");
        return 2;
    }
    if (dim != 2 && dim != 3) {
        printf("Dimenstion must be only 2 or 3");
        return 3;
    }
    
    dim_num = power(2, dim);
    
    // array input
    // N (dim = 2 : N = 0-3, dim = 3 : N = 0-7 )
    // x = N % 2
    // y = (N / 2) % 2
    // z = (N / 4) % 2
    // N - x;y[;z]
    // 0 - 0;0[;0]
    // 1 - 1;0[;0]
    // 2 - 0;1[;0]
    // 3 - 1;1[;0]
    // 4-7 > z = 1
    std::string s = argv[2];
    std::vector<uint> parts;
    int read;
    
    try {
        for (uint i = 0; i < s.size(); ++i) {
            read = s[i] - '0';
            if (read < 0 || read >= dim_num) {
                printf("Error array: for %d input array from 0 to %d", dim, (dim_num-1));
                return 4;
            }
            parts.push_back(read);
        }
    } catch (std::exception &e) {
        printf("Error array: for %d input array from 0 to %d", dim, (dim_num-1));
        return 5;
    }
    
    unsigned t0 = clock();
    
    calculation(parts);
    
    std::cout << "Прошло: " << double(clock() - t0) / CLOCKS_PER_SEC << " сек" << std::endl;
    return 0;
}
