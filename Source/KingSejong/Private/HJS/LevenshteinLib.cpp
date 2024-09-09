// Fill out your copyright notice in the Description page of Project Settings.


#include "HJS/LevenshteinLib.h"

int32 ULevenshteinLib::CalculateLevenshteinDist(const FString& Orignal, const FString& Compare)
{
    int32 Len1 = Orignal.Len();
    int32 Len2 = Compare.Len();

    // 2D 배열을 벡터로 초기화
    TArray<TArray<int32>> DistanceMatrix;
    DistanceMatrix.SetNum(Len1 + 1);

    for ( int32 i = 0; i <= Len1; ++i )
    {
        DistanceMatrix[ i ].SetNum(Len2 + 1);
        DistanceMatrix[ i ][ 0 ] = i;
    }

    for ( int32 j = 0; j <= Len2; ++j )
    {
        DistanceMatrix[ 0 ][ j ] = j;
    }

    // Levenshtein 거리 계산
    for ( int32 i = 1; i <= Len1; ++i )
    {
        for ( int32 j = 1; j <= Len2; ++j )
        {
            int32 Cost = (Orignal[ i - 1 ] == Compare[ j - 1 ]) ? 0 : 1;
            DistanceMatrix[ i ][ j ] = FMath::Min3(
                DistanceMatrix[ i - 1 ][ j ] + 1,        // 삭제
                DistanceMatrix[ i ][ j - 1 ] + 1,        // 삽입
                DistanceMatrix[ i - 1 ][ j - 1 ] + Cost  // 교체
            );
        }
    }

    return DistanceMatrix[ Len1 ][ Len2 ];
}
