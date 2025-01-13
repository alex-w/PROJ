/* enable predefined math constants M_* for MS Visual Studio */
#if defined(_MSC_VER) || defined(_WIN32)
#ifndef _USE_MATH_DEFINES
#define _USE_MATH_DEFINES
#endif
#endif

#include <cmath>
#include <cstdint>
#include <errno.h>

#include "proj.h"
#include "proj_internal.h"

PROJ_HEAD(airocean, "Airocean") "\n\tMisc, Sph&Ell";

namespace { // anonymous namespace
struct pj_face {
    PJ_XYZ p1;
    PJ_XYZ p2;
    PJ_XYZ p3;
};
} // namespace

/*
    The vertices of the faces of the icosahedron are inspired by those used by
   Robert W. Gray.

    Original Reference:
    Robert W. Gray (1995) Exact Transformation Equations for
        Fuller's World Map, Vol. 32. Autumn, 1995, pp. 17-25.

    To accommodate for land parts that would be interrupted by using a mere
   icosahedron, some faces are split in two (Australia) and 3 (Japan) subfaces.

    The parameters below were computed using the script located at:
    scripts/build_airocean_parameters.py
    (relative to the root of the project)
*/
// Define the 23 faces and subfaces
constexpr pj_face base_ico_faces[23] = {
    {{0.42015242670871, 0.07814524940278296, 0.9040825506150193},
     {0.5188367303273644, 0.8354203803782358, 0.18133183755726245},
     {0.9950094394362416, -0.09134779527642793, 0.040147175877166645}},
    {{0.42015242670871, 0.07814524940278296, 0.9040825506150193},
     {-0.4146822253203352, 0.6559624054348008, 0.6306758078914754},
     {0.5188367303273644, 0.8354203803782358, 0.18133183755726245}},
    {{0.42015242670871, 0.07814524940278296, 0.9040825506150193},
     {-0.5154559599440418, -0.381716898287133, 0.7672009925177475},
     {-0.4146822253203352, 0.6559624054348008, 0.6306758078914754}},
    {{0.42015242670871, 0.07814524940278296, 0.9040825506150193},
     {0.3557814025329447, -0.8435800024661781, 0.40223422660292557},
     {-0.5154559599440418, -0.381716898287133, 0.7672009925177475}},
    {{0.42015242670871, 0.07814524940278296, 0.9040825506150193},
     {0.9950094394362416, -0.09134779527642793, 0.040147175877166645},
     {0.3557814025329447, -0.8435800024661781, 0.40223422660292557}},
    {{0.9950094394362416, -0.09134779527642793, 0.040147175877166645},
     {0.5188367303273644, 0.8354203803782358, 0.18133183755726245},
     {0.5154559599440418, 0.381716898287133, -0.7672009925177475}},
    {{0.5154559599440418, 0.381716898287133, -0.7672009925177475},
     {0.5188367303273644, 0.8354203803782358, 0.18133183755726245},
     {-0.3557814025329447, 0.8435800024661781, -0.40223422660292557}},
    {{-0.3557814025329447, 0.8435800024661781, -0.40223422660292557},
     {0.5188367303273644, 0.8354203803782358, 0.18133183755726245},
     {-0.4146822253203352, 0.6559624054348008, 0.6306758078914754}},
    {{-0.5154559599440418, -0.381716898287133, 0.7672009925177475},
     {-0.9950094394362416, 0.09134779527642793, -0.040147175877166645},
     {-0.4146822253203352, 0.6559624054348008, 0.6306758078914754}},
    {{-0.5154559599440418, -0.381716898287133, 0.7672009925177475},
     {-0.5188367303273644, -0.8354203803782358, -0.18133183755726245},
     {-0.9950094394362416, 0.09134779527642793, -0.040147175877166645}},
    {{-0.5154559599440418, -0.381716898287133, 0.7672009925177475},
     {0.3557814025329447, -0.8435800024661781, 0.40223422660292557},
     {-0.5188367303273644, -0.8354203803782358, -0.18133183755726245}},
    {{-0.5188367303273644, -0.8354203803782358, -0.18133183755726245},
     {0.3557814025329447, -0.8435800024661781, 0.40223422660292557},
     {0.4146822253203352, -0.6559624054348008, -0.6306758078914754}},
    {{0.4146822253203352, -0.6559624054348008, -0.6306758078914754},
     {0.3557814025329447, -0.8435800024661781, 0.40223422660292557},
     {0.9950094394362416, -0.09134779527642793, 0.040147175877166645}},
    {{0.5154559599440418, 0.381716898287133, -0.7672009925177475},
     {0.4146822253203352, -0.6559624054348008, -0.6306758078914754},
     {0.9950094394362416, -0.09134779527642793, 0.040147175877166645}},
    {{-0.42015242670871, -0.07814524940278296, -0.9040825506150193},
     {-0.3557814025329447, 0.8435800024661781, -0.40223422660292557},
     {-0.9950094394362416, 0.09134779527642793, -0.040147175877166645}},
    {{-0.42015242670871, -0.07814524940278296, -0.9040825506150193},
     {-0.9950094394362416, 0.09134779527642793, -0.040147175877166645},
     {-0.5188367303273644, -0.8354203803782358, -0.18133183755726245}},
    {{-0.42015242670871, -0.07814524940278296, -0.9040825506150193},
     {-0.5188367303273644, -0.8354203803782358, -0.18133183755726245},
     {0.4146822253203352, -0.6559624054348008, -0.6306758078914754}},
    {{-0.42015242670871, -0.07814524940278296, -0.9040825506150193},
     {0.4146822253203352, -0.6559624054348008, -0.6306758078914754},
     {0.5154559599440418, 0.381716898287133, -0.7672009925177475}},
    {{-0.3557814025329447, 0.8435800024661781, -0.40223422660292557},
     {-0.38796691462082733, 0.3827173765316976, -0.6531583886089725},
     {0.5154559599440418, 0.381716898287133, -0.7672009925177475}},
    {{-0.42015242670871, -0.07814524940278296, -0.9040825506150193},
     {0.5154559599440418, 0.381716898287133, -0.7672009925177475},
     {-0.38796691462082733, 0.3827173765316976, -0.6531583886089725}},
    {{-0.9950094394362416, 0.09134779527642793, -0.040147175877166645},
     {-0.3557814025329447, 0.8435800024661781, -0.40223422660292557},
     {-0.5884910224298405, 0.5302967343924689, 0.06276480180379439}},
    {{-0.3557814025329447, 0.8435800024661781, -0.40223422660292557},
     {-0.4146822253203352, 0.6559624054348008, 0.6306758078914754},
     {-0.5884910224298405, 0.5302967343924689, 0.06276480180379439}},
    {{-0.9950094394362416, 0.09134779527642793, -0.040147175877166645},
     {-0.5884910224298405, 0.5302967343924689, 0.06276480180379439},
     {-0.4146822253203352, 0.6559624054348008, 0.6306758078914754}}};
// // Define the centers for each face or subface
constexpr PJ_XYZ base_ico_centers[23] = {
    {0.6446661988241054, 0.27407261150153034, 0.37518718801648276},
    {0.17476897723857973, 0.5231760117386065, 0.5720300653545857},
    {-0.16999525285188902, 0.1174635855168169, 0.7673197836747474},
    {0.08682595643253761, -0.3823838837835094, 0.6911725899118975},
    {0.5903144228926321, -0.28559418277994103, 0.44882131769837047},
    {0.6764340432358825, 0.375263161129647, -0.18190732636110615},
    {0.22617042924615385, 0.6869057603771823, -0.3293677938544702},
    {-0.0838756325086385, 0.778320929426405, 0.13659113961527075},
    {-0.6417158749002062, 0.12186443414136523, 0.45257654151068544},
    {-0.6764340432358825, -0.375263161129647, 0.18190732636110615},
    {-0.22617042924615385, -0.6869057603771823, 0.32936779385447024},
    {0.0838756325086385, -0.778320929426405, -0.13659113961527075},
    {0.5884910224298405, -0.5302967343924689, -0.06276480180379439},
    {0.6417158749002062, -0.12186443414136523, -0.4525765415106855},
    {-0.5903144228926321, 0.28559418277994103, -0.44882131769837047},
    {-0.6446661988241054, -0.2740726115015303, -0.37518718801648276},
    {-0.17476897723857973, -0.5231760117386065, -0.5720300653545857},
    {0.16999525285188902, -0.11746358551681692, -0.7673197836747474},
    {-0.07609745240324339, 0.5360047590950029, -0.6075312025765486},
    {-0.09755446046183185, 0.2287630084720159, -0.7748139772472463},
    {-0.646427288133009, 0.48840817737835834, -0.12653886689209928},
    {-0.4529848834277068, 0.6766130474311494, 0.09706879436411474},
    {-0.6660608957288058, 0.4258689783678992, 0.2177644779393677}};
// // Define the normals for each face and subface
constexpr PJ_XYZ base_ico_normals[23] = {
    {0.8112534709140969, 0.34489532376393844, 0.47213877364139306},
    {0.21993077914046083, 0.6583691780274996, 0.7198475378926182},
    {-0.21392348345014195, 0.1478171829550702, 0.9656017935214206},
    {0.10926252787847963, -0.4811951572873208, 0.8697775121287253},
    {0.7428567301586793, -0.35939416782780276, 0.5648005936517034},
    {0.8512303986474292, 0.4722343788582682, -0.2289137388687808},
    {0.2846148069787909, 0.8644080972654203, -0.4144792552473538},
    {-0.10554981496139187, 0.9794457296411412, 0.17188746100093646},
    {-0.8075407579970092, 0.15335524858988167, 0.5695261994882688},
    {-0.8512303986474292, -0.4722343788582682, 0.22891373886878083},
    {-0.2846148069787909, -0.8644080972654203, 0.4144792552473538},
    {0.10554981496139185, -0.9794457296411412, -0.17188746100093638},
    {0.7405621473854482, -0.6673299564565524, -0.07898376463267347},
    {0.8075407579970092, -0.15335524858988167, -0.5695261994882688},
    {-0.7428567301586793, 0.35939416782780276, -0.5648005936517034},
    {-0.8112534709140969, -0.34489532376393844, -0.47213877364139306},
    {-0.21993077914046083, -0.6583691780274996, -0.7198475378926182},
    {0.21392348345014195, -0.1478171829550702, -0.9656017935214206},
    {-0.10926252787847963, 0.4811951572873209, -0.8697775121287253},
    {-0.10926252787847968, 0.4811951572873209, -0.8697775121287253},
    {-0.740562147385448, 0.6673299564565524, 0.07898376463267354},
    {-0.7405621473854481, 0.6673299564565524, 0.07898376463267347},
    {-0.7405621473854481, 0.6673299564565525, 0.07898376463267329}};

// /*
//     The points of the Airocean projection map are deduced from the unfolded
//     net of the altered icosahedron defined above. The distances in the
//     projected 2d space are expressed in meter.
// */
// // Define the 23 unfolded surfaces used (from icosahedron + split faces)
constexpr pj_face base_airocean_faces[23] = {
    {{1.8211859946200586, 3.1543866727148018, 1.0},
     {1.8211859946200586, 4.205848896953069, 1.0},
     {2.7317789919300877, 3.6801177848339353, 1.0}},
    {{1.8211859946200586, 3.1543866727148018, 1.0},
     {0.9105929973100293, 3.6801177848339353, 1.0},
     {1.8211859946200586, 4.205848896953069, 1.0}},
    {{1.8211859946200586, 3.1543866727148018, 1.0},
     {0.9105929973100293, 2.628655560595668, 1.0},
     {0.9105929973100293, 3.6801177848339353, 1.0}},
    {{1.8211859946200586, 3.1543866727148018, 1.0},
     {1.8211859946200586, 2.1029244484765344, 1.0},
     {0.9105929973100293, 2.628655560595668, 1.0}},
    {{1.8211859946200586, 3.1543866727148018, 1.0},
     {2.7317789919300877, 3.6801177848339353, 1.0},
     {2.7317789919300877, 2.628655560595668, 1.0}},
    {{2.7317789919300877, 3.6801177848339353, 1.0},
     {1.8211859946200586, 4.205848896953069, 1.0},
     {2.7317789919300877, 4.731580009072203, 1.0}},
    {{1.8211859946200586, 5.257311121191336, 1.0},
     {1.8211859946200586, 4.205848896953069, 1.0},
     {0.9105929973100293, 4.731580009072203, 1.0}},
    {{0.9105929973100293, 4.731580009072203, 1.0},
     {1.8211859946200586, 4.205848896953069, 1.0},
     {0.9105929973100293, 3.6801177848339353, 1.0}},
    {{0.9105929973100293, 2.628655560595668, 1.0},
     {0.0, 3.1543866727148018, 1.0},
     {0.9105929973100293, 3.6801177848339353, 1.0}},
    {{0.9105929973100293, 2.628655560595668, 1.0},
     {0.9105929973100293, 1.5771933363574009, 1.0},
     {0.0, 2.1029244484765344, 1.0}},
    {{0.9105929973100293, 2.628655560595668, 1.0},
     {1.8211859946200586, 2.1029244484765344, 1.0},
     {0.9105929973100293, 1.5771933363574009, 1.0}},
    {{0.9105929973100293, 1.5771933363574009, 1.0},
     {1.8211859946200586, 2.1029244484765344, 1.0},
     {1.8211859946200586, 1.0514622242382672, 1.0}},
    {{1.8211859946200586, 1.0514622242382672, 1.0},
     {1.8211859946200586, 2.1029244484765344, 1.0},
     {2.7317789919300877, 1.5771933363574009, 1.0}},
    {{1.8211859946200586, 0.0, 1.0},
     {1.8211859946200586, 1.0514622242382672, 1.0},
     {2.7317789919300877, 0.5257311121191336, 1.0}},
    {{0.0, 5.257311121191336, 1.0},
     {0.9105929973100293, 4.731580009072203, 1.0},
     {0.0, 4.205848896953069, 1.0}},
    {{0.0, 1.0514622242382672, 1.0},
     {0.0, 2.1029244484765344, 1.0},
     {0.9105929973100293, 1.5771933363574009, 1.0}},
    {{0.9105929973100293, 0.5257311121191336, 1.0},
     {0.9105929973100293, 1.5771933363574009, 1.0},
     {1.8211859946200586, 1.0514622242382672, 1.0}},
    {{0.9105929973100293, 0.5257311121191336, 1.0},
     {1.8211859946200586, 1.0514622242382672, 1.0},
     {1.8211859946200586, 0.0, 1.0}},
    {{0.9105929973100293, 4.731580009072203, 1.0},
     {0.45529649865501465, 4.994445565131769, 1.0},
     {0.9105929973100293, 5.78304223331047, 1.0}},
    {{0.9105929973100293, 0.5257311121191336, 1.0},
     {1.8211859946200586, 0.0, 1.0},
     {0.9105929973100293, 0.0, 1.0}},
    {{0.0, 4.205848896953069, 1.0},
     {0.9105929973100293, 4.731580009072203, 1.0},
     {0.6070619982066862, 4.205848896953069, 1.0}},
    {{0.9105929973100293, 4.731580009072203, 1.0},
     {0.9105929973100293, 3.6801177848339353, 1.0},
     {0.6070619982066862, 4.205848896953069, 1.0}},
    {{0.0, 3.1543866727148018, 1.0},
     {0.3035309991033431, 3.6801177848339353, 1.0},
     {0.9105929973100293, 3.6801177848339353, 1.0}}};

// /*
//     The parameters here are extracted from the transition matrices
//     that allow converting a icosahedron face or subface to its
//     corresponding face in the Airocean projected space.
//     Since only a few parameters of those matrices are relevant,
//     the irrelevant ones has been discarded.
// */
// // Icosahedron to Airocean (forward)
constexpr double base_ico_air_trans[23][4][4] = {
    {{0.5771127852625935, -0.6019490725122667, -0.5519041105011566,
      2.1247169937234016},
     {0.09385435001257117, 0.7202114479424703, -0.6873767753105484,
      3.6801177848339357},
     {0.8112534709140967, 0.3448953237639384, 0.4721387736413929,
      -0.7946544722917659},
     {0.0, 0.0, 0.0, 1.0}},
    {{0.9709901201198636, -0.2187361325341673, -0.09660585361978567,
      1.5176549955167156},
     {0.09385435001257089, 0.7202114479424708, -0.687376775310548,
      3.6801177848339353},
     {0.21993077914046077, 0.6583691780274995, 0.7198475378926181,
      -0.7946544722917659},
     {0.0, 0.0, 0.0, 1.0}},
    {{0.9721374115064793, -0.06476823821979226, 0.2252863255224028,
      1.2141239964133725},
     {0.09584151698527507, 0.9868916636293633, -0.12984316647721492,
      3.1543866727148013},
     {-0.21392348345014195, 0.1478171829550702, 0.9656017935214207,
      -0.7946544722917663},
     {0.0, 0.0, 0.0, 1.0}},
    {{0.9921258753731454, -0.0010987106726278763, -0.1252399307326839,
      1.5176549955167151},
     {0.06122048200295415, 0.8766128070237673, 0.47728611874350335,
      2.6286555605956674},
     {0.10926252787847969, -0.4811951572873209, 0.8697775121287256,
      -0.7946544722917663},
     {0.0, 0.0, 0.0, 1.0}},
    {{0.28030414798915965, -0.5991800396948614, -0.7499419075177327,
      2.428247992826745},
     {0.6079419898954396, 0.7154153424148981, -0.34436524905882676,
      3.1543866727148013},
     {0.742856730158679, -0.3593941678278027, 0.5648005936517032,
      -0.794654472291766},
     {0.0, 0.0, 0.0, 1.0}},
    {{0.25960661905056537, -0.7580069591045613, -0.5983559586852888,
      2.428247992826744},
     {-0.4560824615830708, 0.4499112594427941, -0.7678337364709403,
      4.205848896953069},
     {0.8512303986474292, 0.47223437885826813, -0.22891373886878083,
      -0.7946544722917661},
     {0.0, 0.0, 0.0, 1.0}},
    {{0.958636570067365, -0.258086064605963, 0.12003128669511368,
      1.5176549955167158},
     {-0.003215303703157293, -0.43149765310854393, -0.9021083289627215,
      4.731580009072202},
     {0.2846148069787908, 0.8644080972654206, -0.41447925524735385,
      -0.7946544722917662},
     {0.0, 0.0, 0.0, 1.0}},
    {{0.992834940445074, 0.09405868118990741, 0.07370037668995856,
      1.2141239964133723},
     {0.056018011327093935, 0.17843493822832973, -0.982355819052552,
      4.205848896953068},
     {-0.10554981496139189, 0.9794457296411413, 0.17188746100093644,
      -0.7946544722917662},
     {0.0, 0.0, 0.0, 1.0}},
    {{0.5819727895662967, 0.05026939415592827, 0.8116530417706931,
      0.6070619982066865},
     {0.09584151698527442, 0.9868916636293634, -0.1298431664772149,
      3.1543866727148013},
     {-0.8075407579970093, 0.1533552485898817, 0.5695261994882689,
      -0.7946544722917663},
     {0.0, 0.0, 0.0, 1.0}},
    {{0.5247823074767625, -0.7686380596783918, 0.36578554232391575,
      0.6070619982066867},
     {0.0032153037031566203, 0.4314976531085445, 0.9021083289627214,
      2.102924448476535},
     {-0.8512303986474292, -0.47223437885826813, 0.22891373886878078,
      -0.7946544722917661},
     {0.0, 0.0, 0.0, 1.0}},
    {{0.9586365700673652, -0.2580860646059632, 0.12003128669511379,
      1.2141239964133719},
     {0.003215303703156878, 0.43149765310854465, 0.9021083289627218,
      2.1029244484765344},
     {-0.2846148069787909, -0.8644080972654204, 0.4144792552473538,
      -0.7946544722917662},
     {0.0, 0.0, 0.0, 1.0}},
    {{0.9928349404450738, 0.0940586811899076, 0.07370037668995869,
      1.5176549955167153},
     {-0.05601801132709388, -0.17843493822832968, 0.9823558190525513,
      1.5771933363574009},
     {0.10554981496139189, -0.9794457296411413, -0.17188746100093644,
      -0.7946544722917662},
     {0.0, 0.0, 0.0, 1.0}},
    {{0.6696489291164518, 0.7230710214322986, 0.1695246580826539,
      2.1247169937234016},
     {-0.05601801132709365, -0.17843493822832943, 0.9823558190525516,
      1.5771933363574009},
     {0.7405621473854482, -0.6673299564565524, -0.07898376463267347,
      -0.7946544722917662},
     {0.0, 0.0, 0.0, 1.0}},
    {{0.5819727895662965, 0.050269394155928314, 0.811653041770693,
      2.1247169937234016},
     {-0.09584151698527484, -0.9868916636293626, 0.129843166477215,
      0.5257311121191333},
     {0.8075407579970093, -0.1533552485898817, -0.5695261994882689,
      -0.7946544722917663},
     {0.0, 0.0, 0.0, 1.0}},
    {{0.3863411332821331, 0.9191578806358752, 0.07674189989336716,
      0.30353099910334314},
     {0.5467215078924839, -0.16119746460886833, -0.8216513678023304,
      4.731580009072203},
     {-0.742856730158679, 0.35939416782780265, -0.5648005936517032,
      -0.794654472291766},
     {0.0, 0.0, 0.0, 1.0}},
    {{0.20727614126473407, -0.9246959462706865, 0.31933369413978446,
      0.303530999103343},
     {-0.5467215078924849, 0.1611974646088691, 0.8216513678023303,
      1.5771933363574007},
     {-0.8112534709140967, -0.34489532376393833, -0.47213877364139295,
      -0.794654472291766},
     {0.0, 0.0, 0.0, 1.0}},
    {{0.9709901201198639, -0.21873613253416718, -0.09660585361978535,
      1.2141239964133725},
     {-0.09385435001257073, -0.7202114479424704, 0.6873767753105484,
      1.0514622242382674},
     {-0.21993077914046086, -0.6583691780274995, -0.719847537892618,
      -0.794654472291766},
     {0.0, 0.0, 0.0, 1.0}},
    {{0.9721374115064794, -0.0647682382197923, 0.2252863255224031,
      1.5176549955167156},
     {-0.09584151698527477, -0.9868916636293626, 0.12984316647721514,
      0.5257311121191336},
     {0.21392348345014198, -0.1478171829550702, -0.9656017935214205,
      -0.7946544722917661},
     {0.0, 0.0, 0.0, 1.0}},
    {{0.5490814303330593, 0.7586196048290541, 0.350721938339208,
      0.6070619982066862},
     {0.8285959708235409, -0.43925791486578636, -0.3471040209544599,
      5.257311121191335},
     {-0.10926252787847968, 0.48119515728732093, -0.8697775121287254,
      -0.7946544722917663},
     {0.0, 0.0, 0.0, 1.0}},
    {{0.9921258753731453, -0.0010987106726278503, -0.125239930732684,
      1.2141239964133725},
     {-0.061220482002954366, -0.8766128070237673, -0.4772861187435034, 0.0},
     {-0.10926252787847965, 0.48119515728732093, -0.8697775121287254,
      -0.7946544722917663},
     {0.0, 0.0, 0.0, 1.0}},
    {{0.6696489291164521, 0.7230710214322988, 0.169524658082654,
      0.607061998206686},
     {0.05601801132709396, 0.17843493822832968, -0.9823558190525518,
      4.205848896953069},
     {-0.7405621473854482, 0.6673299564565525, 0.07898376463267334,
      -0.7946544722917662},
     {0.0, 0.0, 0.0, 1.0}},
    {{0.6696489291164525, 0.7230710214322987, 0.1695246580826538,
      0.6070619982066863},
     {0.05601801132709517, 0.1784349382283307, -0.9823558190525518,
      4.205848896953069},
     {-0.7405621473854483, 0.6673299564565526, 0.07898376463267348,
      -0.7946544722917663},
     {0.0, 0.0, 0.0, 1.0}},
    {{0.28631144367947836, 0.20700632128770896, 0.9355074238963061,
      0.3035309991033428},
     {0.6079419898954391, 0.7154153424148978, -0.3443652490588263,
      3.6801177848339357},
     {-0.7405621473854481, 0.6673299564565525, 0.07898376463267341,
      -0.7946544722917661},
     {0.0, 0.0, 0.0, 1.0}}};
// // Airocean to Icosahedron (inverse)
constexpr double base_air_ico_trans[23][4][4] = {
    {{0.577112785262594, 0.09385435001257074, 0.8112534709140972,
      -0.9269302059836626},
     {-0.6019490725122669, 0.7202114479424705, 0.3448953237639385,
      -1.0974189231897016},
     {-0.5519041105011576, -0.6873767753105482, 0.47213877364139284,
      4.0774547262062395},
     {0.0, 0.0, 0.0, 1.0}},
    {{0.970990120119864, 0.09385435001257075, 0.21993077914046097,
      -1.6442540918239978},
     {-0.21873613253416777, 0.7202114479424705, 0.6583691780274992,
      -1.7953209624349933},
     {-0.09660585361978517, -0.6873767753105485, 0.7198475378926187,
      3.248271917398959},
     {0.0, 0.0, 0.0, 1.0}},
    {{0.9721374115064793, 0.09584151698527468, -0.21392348345014173,
      -1.6526118158442071},
     {-0.06476823821979319, 0.9868916636293628, 0.14781718295507035,
      -2.916937653420916},
     {0.22528632552240307, -0.12984316647721503, 0.9656017935214205,
      0.9033698036730199},
     {0.0, 0.0, 0.0, 1.0}},
    {{0.9921258753731454, 0.061220482002954296, 0.10926252787847969,
      -1.5798063949483236},
     {-0.0010987106726281115, 0.8766128070237668, -0.48119515728732043,
      -2.68502954971497},
     {-0.12523993073268413, 0.4772861187435032, 0.8697775121287253,
      -0.3733772136037109},
     {0.0, 0.0, 0.0, 1.0}},
    {{0.2803041479891603, 0.6079419898954391, 0.7428567301586791,
      -2.0080176725529477},
     {-0.5991800396948611, 0.7154153424148979, -0.35939416782780287,
      -1.0873330756182955},
     {-0.7499419075177335, -0.3443652490588265, 0.5648005936517035,
      3.3561274015422433},
     {0.0, 0.0, 0.0, 1.0}},
    {{0.2596066190505654, -0.4560824615830712, 0.8512303986474292,
      1.9642587095706099},
     {-0.7580069591045617, 0.4499112594427949, 0.47223437885826836,
      0.3236332638697585},
     {-0.5983559586852887, -0.7678337364709401, -0.22891373886878078,
      4.500442002892026},
     {0.0, 0.0, 0.0, 1.0}},
    {{0.958636570067365, -0.003215303703156967, 0.28461480697879094,
      -1.2134956834766393},
     {-0.2580860646059631, -0.43149765310854504, 0.8644080972654203,
      3.1202570350096352},
     {0.12003128669511316, -0.9021083289627222, -0.4144792552473535,
      3.756863859611938},
     {0.0, 0.0, 0.0, 1.0}},
    {{0.992834940445074, 0.05601801132709367, -0.10554981496139178,
      -1.5249036493302057},
     {0.09405868118990754, 0.17843493822832954, 0.9794457296411412,
      -0.08634836060276646},
     {0.07370037668995799, -0.9823558190525513, 0.1718874610009362,
      4.17874988170889},
     {0.0, 0.0, 0.0, 1.0}},
    {{0.581972789566297, 0.09584151698527493, -0.8075407579970092,
      -1.297330643307362},
     {0.05026939415592872, 0.986891663629363, 0.15335524858988142,
      -3.0216901158893736},
     {0.8116530417706934, -0.12984316647721506, 0.5695261994882689,
      0.3694283780016493},
     {0.0, 0.0, 0.0, 1.0}},
    {{0.5247823074767624, 0.0032153037031565812, -0.8512303986474291,
      -1.0017709802028867},
     {-0.7686380596783923, 0.431497653108545, -0.47223437885826824,
      -0.8160591689057779},
     {0.36578554232391597, 0.9021083289627221, 0.22891373886878089,
      -1.937212836027187},
     {0.0, 0.0, 0.0, 1.0}},
    {{0.9586365700673654, 0.0032153037031565886, -0.2846148069787907,
      -1.3968356335709964},
     {-0.258086064605963, 0.43149765310854504, -0.8644080972654202,
      -1.2809642403813966},
     {0.12003128669511362, 0.9021083289627224, 0.41447925524735396,
      -1.7134307317924613},
     {0.0, 0.0, 0.0, 1.0}},
    {{0.9928349404450739, -0.05601801132709361, 0.10554981496139221,
      -1.3345540404002831},
     {0.09405868118990741, -0.17843493822832956, -0.9794457296411411,
      -0.639643161258916},
     {0.07370037668995856, 0.982355819052552, -0.17188746100093616,
      -1.7978079362118518},
     {0.0, 0.0, 0.0, 1.0}},
    {{0.6696489291164524, -0.056018011327093886, 0.7405621473854481,
      -0.7459722029114777},
     {0.723071021432299, -0.1784349382283297, -0.6673299564565522,
      -1.7851916257515466},
     {0.16952465808265363, 0.9823558190525522, -0.07898376463267373,
      -1.9723217754287594},
     {0.0, 0.0, 0.0, 1.0}},
    {{0.5819727895662968, -0.09584151698527477, 0.8075407579970091,
      -0.5444247336640644},
     {0.05026939415592821, -0.9868916636293631, -0.15335524858988164,
      0.29016698169232114},
     {0.8116530417706935, 0.1298431664772151, -0.569526199488269,
      -2.2453721446813035},
     {0.0, 0.0, 0.0, 1.0}},
    {{0.3863411332821329, 0.5467215078924852, -0.7428567301586795,
      -3.2944374903463687},
     {0.9191578806358753, -0.16119746460886916, 0.3593941678278029,
      0.7693199739932717},
     {0.07674189989336772, -0.8216513678023304, -0.564800593651703,
      3.4155943230742447},
     {0.0, 0.0, 0.0, 1.0}},
    {{0.20727614126473443, -0.546721507892485, -0.8112534709140969,
      0.15470458601882164},
     {-0.9246959462706867, 0.16119746460886913, -0.3448953237639384,
      -0.24763829408199384},
     {0.31933369413978435, 0.8216513678023302, -0.4721387736413931,
      -1.768017925352872},
     {0.0, 0.0, 0.0, 1.0}},
    {{0.9709901201198642, -0.09385435001257068, -0.21993077914046077,
      -1.2549870787377553},
     {-0.2187361325341676, -0.7202114479424703, -0.6583691780274997,
      0.4996719066292349},
     {-0.09660585361978546, 0.6873767753105482, -0.7198475378926181,
      -1.1774892933385632},
     {0.0, 0.0, 0.0, 1.0}},
    {{0.9721374115064794, -0.09584151698527459, 0.21392348345014212,
      -1.2549870787377553},
     {-0.06476823821979266, -0.9868916636293628, -0.14781718295507024,
      0.49967190662923483},
     {0.2252863255224028, 0.12984316647721506, -0.9656017935214204,
      -1.1774892933385632},
     {0.0, 0.0, 0.0, 1.0}},
    {{0.5490814303330579, 0.8285959708235412, -0.10926252787847955,
      -4.776339239093644},
     {0.7586196048290552, -0.4392579148657884, 0.4811951572873209,
      2.231170271492442},
     {0.3507219383392087, -0.3471040209544594, -0.8697775121287253,
      0.9207512789590909},
     {0.0, 0.0, 0.0, 1.0}},
    {{0.9921258753731456, -0.061220482002954546, -0.10926252787847962,
      -1.2913897891856965},
     {-0.00109871067262767, -0.8766128070237672, 0.48119515728732093,
      0.38371785477626236},
     {-0.12523993073268386, -0.47728611874350324, -0.8697775121287252,
      -0.5391157847001973},
     {0.0, 0.0, 0.0, 1.0}},
    {{0.6696489291164526, 0.0560180113270932, -0.7405621473854482,
      -1.2306127305858023},
     {0.723071021432299, 0.17843493822832968, 0.6673299564565522,
      -0.6591225928490807},
     {0.16952465808265377, -0.9823558190525503, 0.07898376463267326,
      4.09149296210043},
     {0.0, 0.0, 0.0, 1.0}},
    {{0.669648929116452, 0.056018011327093706, -0.740562147385448,
      -1.230612730585803},
     {0.7230710214322988, 0.1784349382283296, 0.6673299564565524,
      -0.6591225928490807},
     {0.1695246580826554, -0.9823558190525514, 0.0789837646326731,
      4.091492962100434},
     {0.0, 0.0, 0.0, 1.0}},
    {{0.2863114436794785, 0.6079419898954399, -0.7405621473854486,
      -2.9126935501461353},
     {0.2070063212877089, 0.7154153424148983, 0.6673299564565521,
      -2.165348826292825},
     {0.935507423896306, -0.3443652490588271, 0.07898376463267351,
      1.046113976300111},
     {0.0, 0.0, 0.0, 1.0}}};

// By default the resulting orientation of the projection is vertical
// the following transforms are used to alter the projection data
// so that the resulting orientation is horizontal instead
constexpr double orient_horizontal_trans[4][4] = {
    {0.0, -1.0, 0.0, 5.78304223331047},
    {1.0, 0.0, 0.0, 0.0},
    {0.0, 0.0, 1.0, 0.0},
    {0.0, 0.0, 0.0, 1.0}};
constexpr double orient_horizontal_inv_trans[4][4] = {
    {0.0, 1.0, 0.0, 0.0},
    {-1.0, -0.0, -0.0, 5.78304223331047},
    {0.0, 0.0, 1.0, 0.0},
    {0.0, 0.0, 0.0, 1.0}};

namespace { // anonymous namespace

struct pj_airocean_data {
    pj_face ico_faces[23] = {};
    PJ_XYZ ico_centers[23] = {};
    PJ_XYZ ico_normals[23] = {};
    pj_face airocean_faces[23] = {};
    double ico_air_trans[23][4][4] = {};
    double air_ico_trans[23][4][4] = {};

    void initialize() {
        memcpy((char *)this->ico_faces, (char *)base_ico_faces,
               sizeof(pj_face[23]));
        memcpy((char *)this->airocean_faces, (char *)base_airocean_faces,
               sizeof(pj_face[23]));
        memcpy(this->ico_centers, base_ico_centers, sizeof(PJ_XYZ[23]));
        memcpy(this->ico_normals, base_ico_normals, sizeof(PJ_XYZ[23]));
        memcpy(this->ico_air_trans, base_ico_air_trans,
               sizeof(double[23][4][4]));
        memcpy(this->air_ico_trans, base_air_ico_trans,
               sizeof(double[23][4][4]));
    }

    static void mat_mult(const double m1[4][4], const double m2[4][4],
                         double res[4][4]) {
        for (unsigned char i = 0; i < 4; ++i)
            for (unsigned char j = 0; j < 4; ++j)
                res[i][j] = (m1[i][0] * m2[0][j]) + (m1[i][1] * m2[1][j]) +
                            (m1[i][2] * m2[2][j]) + (m1[i][3] * m2[3][j]);
    }

    static PJ_XYZ vec_mult(const double m[4][4], const PJ_XYZ *v) {
        double x = m[0][0] * v->x + m[0][1] * v->y + m[0][2] * v->z + m[0][3];
        double y = m[1][0] * v->x + m[1][1] * v->y + m[1][2] * v->z + m[1][3];
        double z = m[2][0] * v->x + m[2][1] * v->y + m[2][2] * v->z + m[2][3];
        return {x, y, z};
    }

    void transform(const double m[4][4], const double inv_m[4][4]) {
        for (unsigned char i = 0; i < 23; i++) {
            mat_mult(m, base_ico_air_trans[i], this->ico_air_trans[i]);
            mat_mult(base_air_ico_trans[i], inv_m, this->air_ico_trans[i]);
            this->airocean_faces[i] = {
                vec_mult(m, &base_airocean_faces[i].p1),
                vec_mult(m, &base_airocean_faces[i].p2),
                vec_mult(m, &base_airocean_faces[i].p3),
            };
        }
    }
};

} // anonymous namespace

inline double det(const PJ_XYZ *u, const PJ_XYZ *v, const PJ_XYZ *w) {
    return (u->x * (v->y * w->z - v->z * w->y) -
            v->x * (u->y * w->z - u->z * w->y) +
            w->x * (u->y * v->z - u->z * v->y));
}

inline bool is_point_in_face(const PJ_XYZ *p, const pj_face *face) {
    return (det(p, &face->p2, &face->p3) <= 0 &&
            det(&face->p1, p, &face->p3) <= 0 &&
            det(&face->p1, &face->p2, p) <= 0);
}

inline unsigned char get_ico_face_index(const pj_airocean_data *pj_data,
                                        const PJ_XYZ *p) {
    for (unsigned char i = 0; i < 23; i++) {
        if (is_point_in_face(p, &pj_data->ico_faces[i])) {
            return i;
        }
    }

    return 23;
}

inline unsigned char get_dym_face_index(const pj_airocean_data *pj_data,
                                        const PJ_XY *p) {
    const PJ_XYZ pp{p->x, p->y, 1.0};
    for (unsigned char i = 0; i < 23; i++) {
        if (is_point_in_face(&pp, &pj_data->airocean_faces[i])) {
            return i;
        }
    }

    return 23;
}

inline PJ_XY ico_to_dym(const pj_airocean_data *pj_data, const PJ_XYZ *p,
                        unsigned char face_id) {
    return PJ_XY{
        pj_data->ico_air_trans[face_id][0][0] * p->x +     // * -1
            pj_data->ico_air_trans[face_id][0][1] * p->y + //
            pj_data->ico_air_trans[face_id][0][2] * p->z + //
            pj_data->ico_air_trans[face_id][0][3],         // +1000
        pj_data->ico_air_trans[face_id][1][0] * p->x +
            pj_data->ico_air_trans[face_id][1][1] * p->y +
            pj_data->ico_air_trans[face_id][1][2] * p->z +
            pj_data->ico_air_trans[face_id][1][3],
    };
}

inline PJ_XYZ dym_to_ico(const pj_airocean_data *pj_data, const PJ_XY *p,
                         unsigned char face_id) {
    return PJ_XYZ{
        pj_data->air_ico_trans[face_id][0][0] * p->x +     // * -1
            pj_data->air_ico_trans[face_id][0][1] * p->y + //
            pj_data->air_ico_trans[face_id][0][3], // + [face_id][0][0] * 1000
        pj_data->air_ico_trans[face_id][1][0] * p->x +
            pj_data->air_ico_trans[face_id][1][1] * p->y +
            pj_data->air_ico_trans[face_id][1][3],
        pj_data->air_ico_trans[face_id][2][0] * p->x +
            pj_data->air_ico_trans[face_id][2][1] * p->y +
            pj_data->air_ico_trans[face_id][2][3],
    };
}

inline PJ_XYZ cartesian_to_ico(const pj_airocean_data *pj_data, const PJ_XYZ *p,
                               unsigned char face_id) {
    const PJ_XYZ *center = &pj_data->ico_centers[face_id];
    const PJ_XYZ *normal = &pj_data->ico_normals[face_id];

    // cppcheck-suppress unreadVariable
    double a =
        1.0 - (center->x * normal->x + center->y * normal->y +
               center->z * normal->z) /
                  (p->x * normal->x + p->y * normal->y + p->z * normal->z);

    return PJ_XYZ{
        p->x - a * p->x,
        p->y - a * p->y,
        p->z - a * p->z,
    };
}

// ============================================
//
//      The Forward and Inverse Functions
//
// ============================================
static PJ_XY airocean_forward(PJ_LP lp, PJ *P) {
    const struct pj_airocean_data *Q =
        static_cast<struct pj_airocean_data *>(P->opaque);

    double lat;

    /* Convert the geodetic latitude to a geocentric latitude.
     * This corresponds to the shift from the ellipsoid to the sphere
     * described in [LK12]. */
    if (P->es != 0.0) {
        double one_minus_f = 1.0 - (P->a - P->b) / P->a;
        double one_minus_f_squared = one_minus_f * one_minus_f;
        lat = atan(one_minus_f_squared * tan(lp.phi));
    } else {
        lat = lp.phi;
    }

    // Convert the lat/long to x,y,z on the unit sphere
    double x, y, z;
    double sinlat, coslat;
    double sinlon, coslon;

    sinlat = sin(lat);
    coslat = cos(lat);
    sinlon = sin(lp.lam);
    coslon = cos(lp.lam);
    x = coslat * coslon;
    y = coslat * sinlon;
    z = sinlat;

    PJ_XYZ cartesianPoint{x, y, z};

    unsigned char face_id = get_ico_face_index(Q, &cartesianPoint);

    PJ_XYZ icoPoint = cartesian_to_ico(Q, &cartesianPoint, face_id);

    PJ_XY airoceanPoint = ico_to_dym(Q, &icoPoint, face_id);

    return airoceanPoint;
}

static PJ_LP airocean_inverse(PJ_XY xy, PJ *P) {
    const struct pj_airocean_data *Q =
        static_cast<struct pj_airocean_data *>(P->opaque);

    PJ_LP lp = {0.0, 0.0};

    unsigned char face_id = get_dym_face_index(Q, &xy);

    if (face_id == 23) {
        // Point lies outside icosahedron net faces
        proj_errno_set(P, PROJ_ERR_COORD_TRANSFM_OUTSIDE_PROJECTION_DOMAIN);
        lp.lam = HUGE_VAL;
        lp.phi = HUGE_VAL;
        return lp;
    }

    PJ_XYZ sphereCoords = dym_to_ico(Q, &xy, face_id);

    double norm = sqrt((sphereCoords.x * sphereCoords.x) +
                       (sphereCoords.y * sphereCoords.y) +
                       (sphereCoords.z * sphereCoords.z));
    double q = sphereCoords.x / norm;
    double r = sphereCoords.y / norm;
    double s = sphereCoords.z / norm;

    // Get the spherical angles from the x y z
    lp.phi = acos(-s) - M_HALFPI;
    lp.lam = atan2(r, q);

    /* Apply the shift from the sphere to the ellipsoid as described
     * in [LK12]. */
    if (P->es != 0.0) {
        int invert_sign;
        volatile double tanphi, xa;
        invert_sign = (lp.phi < 0.0 ? 1 : 0);
        tanphi = tan(lp.phi);
        double one_minus_f = 1.0 - (P->a - P->b) / P->a;
        double a_squared = P->a * P->a;
        xa = P->b / sqrt(tanphi * tanphi + one_minus_f * one_minus_f);
        lp.phi = atan(sqrt(a_squared - xa * xa) / (one_minus_f * xa));
        if (invert_sign) {
            lp.phi = -lp.phi;
        }
    }

    return lp;
}

PJ *PJ_PROJECTION(airocean) {
    char *opt;
    struct pj_airocean_data *Q = static_cast<struct pj_airocean_data *>(
        calloc(1, sizeof(struct pj_airocean_data)));
    if (nullptr == Q)
        return pj_default_destructor(P, PROJ_ERR_OTHER /*ENOMEM*/);
    Q->initialize();
    P->opaque = Q;
    opt = pj_param(P->ctx, P->params, "sorient").s;
    if (opt) {
        if (!strcmp(opt, "horizontal")) {
            Q->transform(orient_horizontal_trans, orient_horizontal_inv_trans);
        } else if (!strcmp(opt, "vertical")) {
            // the orientation is vertical by default.
        } else {
            proj_log_error(P, _("Invalid value for orient: only vertical or "
                                "horizontal are supported"));
            return pj_default_destructor(P,
                                         PROJ_ERR_INVALID_OP_ILLEGAL_ARG_VALUE);
        }
    }

    P->inv = airocean_inverse;
    P->fwd = airocean_forward;

    return P;
}
