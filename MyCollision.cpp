﻿#include "MyCollision.h"

/// <summary>
/// 球の当たり判定をとる関数
/// </summary>
/// <param name="s1">球1</param>
/// <param name="s2">球2</param>
/// <returns>衝突しているか</returns>
bool MyCollision::IsCollisionSphere(const Sphere& s1, const Sphere& s2) {

	// 計算用
	float distance{};

	// 2つの球の中心座標の距離をとる
	distance = MyMath::Length(s1.center - s2.center);

	// 衝突しているか
	if (distance <= s1.radius + s2.radius) {
		// 衝突していたらTrueを返す
		return true;
	}
	
	// していない場合はFalse
	return false;

}

/// <summary>
/// 球と平面の当たり判定をとる関数
/// </summary>
/// <param name="s">球</param>
/// <param name="p">平面</param>
/// <returns>衝突しているか</returns>
bool MyCollision::IsCollisionPlane(const Sphere& s, const Plane& p) {

	// 計算用
	float k{};

	// 計算処理
	k = MyMath::Dot(p.normal, s.center) - p.distance;
	// kを絶対値に変換
	k = std::abs(k);

	// 衝突しているか
	if (k <= s.radius) {
		// していたらTrue
		return true;
	}

	// していなければfalse
	return false;

}

/// <summary>
/// 直線と平面の当たり判定をとる関数
/// </summary>
/// <param name="l">直線</param>
/// <param name="p">平面</param>
/// <returns>衝突しているか</returns>
bool MyCollision::IsCollisionLine(const Line& l, const Plane& p) {

	// 垂直な判定を散るために法線と線の内積をとる
	float dot = MyMath::Dot(p.normal, l.diff);

	// 垂直な場合は平行であるため衝突はしていない
	if (dot == 0.0f) {
		return false;
	}

	// tを求める
	float t = (p.distance - MyMath::Dot(l.origin, p.normal)) / dot;

	// tの計算結果と線の種類によって衝突しているか検証する
	return true;

}

/// <summary>
/// 半直線と平面の当たり判定をとる関数
/// </summary>
/// <param name="r">半直線</param>
/// <param name="p">平面</param>
/// <returns>衝突しているか</returns>
bool MyCollision::IsCollisionLine(const Ray& r, const Plane& p) {

	// 垂直な判定を散るために法線と線の内積をとる
	float dot = MyMath::Dot(p.normal, r.diff);

	// 垂直な場合は平行であるため衝突はしていない
	if (dot == 0.0f) {
		return false;
	}

	// tを求める
	float t = (p.distance - MyMath::Dot(r.origin, p.normal)) / dot;

	// tの計算結果と線の種類によって衝突しているか検証する
	if (t < 1.0f) {
		return false;
	}
	else {
		return true;
	}

}

/// <summary>
/// 線分と平面の当たり判定をとる関数
/// </summary>
/// <param name="s">線分</param>
/// <param name="p">平面</param>
/// <returns>衝突しているか</returns>
bool MyCollision::IsCollisionLine(const Segment& s, const Plane& p) {

	// 垂直な判定を散るために法線と線の内積をとる
	float dot = MyMath::Dot(p.normal, s.diff);

	// 垂直な場合は平行であるため衝突はしていない
	if (dot == 0.0f) {
		return false;
	}

	// tを求める
	float t = (p.distance - MyMath::Dot(s.origin, p.normal)) / dot;

	// tの計算結果と線の種類によって衝突しているか検証する
	if (t < 0.0f || 1.0f < t) {
		return false;
	}
	else {
		return true;
	}

}

/// <summary>
/// 三角形と線分の当たり判定
/// </summary>
/// <param name="t">三角形</param>
/// <param name="p">平面</param>
/// <returns>衝突しているか</returns>
bool MyCollision::IsCollisionTriangle(const Triangle& triangle, const Segment& s) {

	// 3角形の平面の当たり判定をとる
	Plane plane = {};

	// 3角形を平面に落とし込む
	plane.normal = MyMath::Normalize(
		MyMath::Cross(
			triangle.vertex[1] - triangle.vertex[0],
			triangle.vertex[2] - triangle.vertex[1]
		)
	);

	plane.distance = MyMath::Dot(triangle.vertex[0], plane.normal);

	// 垂直な判定をとるために法線と線の内積をとる
	float dot = MyMath::Dot(plane.normal, s.diff);

	// 垂直な場合は平行であるため衝突はしていない
	if (dot == 0.0f) {
		return false;
	}

	// tを求める
	float t = (plane.distance - MyMath::Dot(s.origin, plane.normal)) / dot;

	if (0.0f < t && 1.0f > t) {
		// 衝突点pを求める
		Vector3 p = s.origin + MyMath::Multiply(t, s.diff);

		// 各辺を結んだベクトルと頂点と衝突点pを結んだベクトルのクロス積をとる
		Vector3 cross01 = MyMath::Cross(
			triangle.vertex[1] - triangle.vertex[0],
			p - triangle.vertex[1]
		);

		Vector3 cross12 = MyMath::Cross(
			triangle.vertex[2] - triangle.vertex[1],
			p - triangle.vertex[2]
		);

		Vector3 cross20 = MyMath::Cross(
			triangle.vertex[0] - triangle.vertex[2],
			p - triangle.vertex[0]
		);

		// 全ての小三角形のクロス積と法線が同じ方向なら衝突している
		if (MyMath::Dot(cross01, plane.normal) >= 0.0f,
			MyMath::Dot(cross12, plane.normal) >= 0.0f,
			MyMath::Dot(cross20, plane.normal) >= 0.0f) {

			return true;

		}

	}

	return false;

}