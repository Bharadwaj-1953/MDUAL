 #include <iostream>
 #include <string>
 #include <unordered_map>
 #include <unordered_set>
 #include <vector>
 #include "../MDUAL/MDUAL.cpp"
 #include "NAIVE.cpp"
 #include "../loader/DataLoader.cpp"
 #include "../loader/QueryLoader.cpp"
 #include "../loader/Query.h"
 #include "../loader/Tuple.h"
 
//  class Tuple {
//     public:
//         int id;
//         int slideID;
//         std::vector<double> value;  // corresponds to t1.value[]
//         std::unordered_set<int> outlierQueryIDs; // store query IDs for which it's an outlier
//         std::vector<short> fullDimCellIdx;
//         std::vector<short> subDimCellIdx;

 int main() {
     try {
         // Equivalent to: String dataset = "FC";
         std::string dataset = "FC";
 
         // Equivalent to: String queryset = dataset+"_Q10";
         std::string queryset = dataset + "_Q10";
 
         int nW = 50;
 
         // DataLoader dLoader = new DataLoader(dataset);
         DataLoader dLoader(dataset);
         // QueryLoader qLoader = new QueryLoader(queryset);
         QueryLoader qLoader(queryset);
 
         // int nS = qLoader.maxW / qLoader.gcdS;
         // int gcdS = qLoader.gcdS;
         int gcdS = qLoader.gcdS;
         int nS   = qLoader.maxW / gcdS;
 
         // MDUAL MDUAL = new MDUAL(dLoader.dim, dLoader.subDim, nS, gcdS, dLoader.getMinValues());
         // NAIVE NAIVE = new NAIVE(nS, gcdS);
         MDUAL md(dLoader.dim, dLoader.subDim, nS, gcdS, dLoader.getMinValues());
         NAIVE nv(nS, gcdS); // We'll call the instance "nv" to avoid shadowing the class name.
 
         for (int itr = 0; itr < nW + nS - 1; itr++) {
             auto newQuerySet = qLoader.getQuerySetByQID(itr * 5, 10);
             if (newQuerySet.empty()) {
                 break;
             }
             auto newSlideTuples_NETSPlus = dLoader.getNewSlideTuples(itr, qLoader.gcdS);
             auto newSlideTuples_NAIVE    = dLoader.getNewSlideTuples(itr, qLoader.gcdS);
 
             if (newSlideTuples_NETSPlus.empty()) {
                 break;
             }
             auto outliers_MDUAL = md.findOutlier(newSlideTuples_NETSPlus, newQuerySet, itr);
             auto outliers_NAIVE = nv.findOutlier(newSlideTuples_NAIVE, newQuerySet, itr);
             std::unordered_map<int, std::unordered_set<int>> outliersMap;
 
             // for (Tuple o: outliers_NAIVE) outliers.put(o.id, o.outlierQueryIDs);
             for (Tuple* o : outliers_NAIVE) {
                outliersMap[o->id] = std::unordered_set<int>(o->outlierQueryIDs.begin(), o->outlierQueryIDs.end());
            }
 
             // Compare outliers from MDUAL vs. NAIVE
             for (auto* o : outliers_MDUAL) {
                 int oid = o->id;
                 if (outliersMap.find(oid) != outliersMap.end()) {
                     // For each qid in o->outlierQueryIDs, remove from outliersMap[oid].
                     for (auto qid : o->outlierQueryIDs) {
                         // removed = outliersMap[oid].remove(qid)
                         size_t countErased = outliersMap[oid].erase(qid);
                         if (countErased == 0) {
                             std::cout << "NETS+ outlier " << oid 
                                       << " having false qid " << qid << std::endl;
                         }
                     }
                     if (outliersMap[oid].empty()) {
                         outliersMap.erase(oid);
                     } else {
                         std::cout << "NETS+ missed " << outliersMap[oid].size() << " qids" 
                                   << std::endl;
                     }
                 } else {
                     // "NETS+ returned false outlier " + o.id
                     std::cout << "NETS+ returned false outlier " << oid << std::endl;
                 }
             }
 
             if (outliersMap.empty()) {
                 std::cout << "Itr " << itr << " clear!" << std::endl;
             } else {
                 std::cout << "At itr " << itr << ", NETS missed { ";
                 for (auto &kv : outliersMap) {
                     std::cout << kv.first << " ";
                 }
                 std::cout << "}" << std::endl;
             }
         }
 
     } catch (const std::exception& ex) {
         std::cerr << "Exception in autoTester main: " << ex.what() << std::endl;
         return 1;
     }
     return 0;
 }
