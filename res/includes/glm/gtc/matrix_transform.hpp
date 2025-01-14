///////////////////////////////////////////////////////////////////////////////////
/// OpenGL Mathematics (glm.g-truc.net)
///
/// Copyright (c) 2005 - 2013 G-Truc Creation (www.g-truc.net)
/// Permission is hereby granted, free of charge, to any person obtaining a copy
/// of this software and associated documentation files (the "Software"), to deal
/// in the Software without restriction, including without limitation the rights
/// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
/// copies of the Software, and to permit persons to whom the Software is
/// furnished to do so, subject to the following conditions:
/// 
/// The above copyright notice and this permission notice shall be included in
/// all copies or substantial portions of the Software.
/// 
/// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
/// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
/// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
/// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
/// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
/// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
/// THE SOFTWARE.
///
/// @ref gtc_matrix_transform
/// @file glm/gtc/matrix_transform.hpp
/// @date 2009-04-29 / 2011-05-16
/// @author Christophe Riccio
///
/// @see core (dependence)
/// @see gtx_transform
/// @see gtx_transform2
/// 
/// @defgroup gtc_matrix_transform GLM_GTC_matrix_transform
/// @ingroup gtc
/// 
/// @brief Defines functions that generate common transformation matrices.
/// 
/// The matrices generated by this extension use standard OpenGL fixed-function
/// conventions. For example, the lookAt function generates a transform from world
/// space into the specific eye space that the projective matrix functions 
/// (perspective, ortho, etc) are designed to expect. The OpenGL compatibility
/// specifications defines the particular layout of this eye space.
/// 
/// <glm/gtc/matrix_transform.hpp> need to be included to use these functionalities.
///////////////////////////////////////////////////////////////////////////////////

#ifndef GLM_GTC_matrix_transform
#define GLM_GTC_matrix_transform

// Dependency:
#include "../mat4x4.hpp"
#include "../vec2.hpp"
#include "../vec3.hpp"
#include "../vec4.hpp"

#if(defined(GLM_MESSAGES) && !defined(GLM_EXT_INCLUDED))
#	pragma message("GLM: GLM_GTC_matrix_transform extension included")
#endif

namespace glm
{
	/// @addtogroup gtc_matrix_transform
	/// @{

	/// Builds a translation 4 * 4 matrix created from a vector of 3 components.
	/// 
	/// @param m Input matrix multiplied by this translation matrix.
	/// @param v Coordinates of a translation vector.
	/// @tparam T Value type used to build the matrix. Currently supported: half (not recommanded), float or double.
	/// @code
	/// #include <glm/glm.hpp>
	/// #include <glm/gtc/matrix_transform.hpp>
	/// ...
	/// glm::mat4 m = glm::translate(glm::mat4(1.0f), glm::vec3(1.0f));
	/// // m[0][0] == 1.0f, m[0][1] == 0.0f, m[0][2] == 0.0f, m[0][3] == 0.0f
	/// // m[1][0] == 0.0f, m[1][1] == 1.0f, m[1][2] == 0.0f, m[1][3] == 0.0f
	/// // m[2][0] == 0.0f, m[2][1] == 0.0f, m[2][2] == 1.0f, m[2][3] == 0.0f
	/// // m[3][0] == 1.0f, m[3][1] == 1.0f, m[3][2] == 1.0f, m[3][3] == 1.0f
	/// @endcode
	/// @see gtc_matrix_transform
	/// @see gtx_transform
	/// @see - translate(T x, T y, T z)
	/// @see - translate(detail::tmat4x4<T, P> const & m, T x, T y, T z)
	/// @see - translate(detail::tvec3<T, P> const & v)
	template <typename T, precision P>
	GLM_FUNC_DECL detail::tmat4x4<T, P> translate(
		detail::tmat4x4<T, P> const & m,
		detail::tvec3<T, P> const & v);
		
	/// Builds a rotation 4 * 4 matrix created from an axis vector and an angle. 
	/// 
	/// @param m Input matrix multiplied by this rotation matrix.
	/// @param angle Rotation angle expressed in radians if GLM_FORCE_RADIANS is define or degrees otherwise.
	/// @param axis Rotation axis, recommanded to be normalized.
	/// @tparam T Value type used to build the matrix. Supported: half, float or double.
	/// @see gtc_matrix_transform
	/// @see gtx_transform
	/// @see - rotate(T angle, T x, T y, T z) 
	/// @see - rotate(detail::tmat4x4<T, P> const & m, T angle, T x, T y, T z) 
	/// @see - rotate(T angle, detail::tvec3<T, P> const & v) 
	template <typename T, precision P>
	GLM_FUNC_DECL detail::tmat4x4<T, P> rotate(
		detail::tmat4x4<T, P> const & m,
		T const & angle,
		detail::tvec3<T, P> const & axis);

	/// Builds a scale 4 * 4 matrix created from 3 scalars. 
	/// 
	/// @param m Input matrix multiplied by this scale matrix.
	/// @param v Ratio of scaling for each axis.
	/// @tparam T Value type used to build the matrix. Currently supported: half (not recommanded), float or double.
	/// @see gtc_matrix_transform
	/// @see gtx_transform
	/// @see - scale(T x, T y, T z) scale(T const & x, T const & y, T const & z)
	/// @see - scale(detail::tmat4x4<T, P> const & m, T x, T y, T z)
	/// @see - scale(detail::tvec3<T, P> const & v)
	template <typename T, precision P>
	GLM_FUNC_DECL detail::tmat4x4<T, P> scale(
		detail::tmat4x4<T, P> const & m,
		detail::tvec3<T, P> const & v);

	/// Creates a matrix for an orthographic parallel viewing volume.
	/// 
	/// @param left 
	/// @param right 
	/// @param bottom 
	/// @param top 
	/// @param zNear 
	/// @param zFar 
	/// @tparam T Value type used to build the matrix. Currently supported: half (not recommanded), float or double.
	/// @see gtc_matrix_transform
	/// @see - glm::ortho(T const & left, T const & right, T const & bottom, T const & top)
	template <typename T>
	GLM_FUNC_DECL detail::tmat4x4<T, defaultp> ortho(
		T const & left,
		T const & right,
		T const & bottom,
		T const & top,
		T const & zNear,
		T const & zFar);

	/// Creates a matrix for projecting two-dimensional coordinates onto the screen.
	/// 
	/// @param left 
	/// @param right 
	/// @param bottom 
	/// @param top 
	/// @tparam T Value type used to build the matrix. Currently supported: half (not recommanded), float or double.
	/// @see gtc_matrix_transform
	/// @see - glm::ortho(T const & left, T const & right, T const & bottom, T const & top, T const & zNear, T const & zFar)
	template <typename T>
	GLM_FUNC_DECL detail::tmat4x4<T, defaultp> ortho(
		T const & left,
		T const & right,
		T const & bottom,
		T const & top);

	/// Creates a frustum matrix.
	/// 
	/// @param left 
	/// @param right 
	/// @param bottom 
	/// @param top 
	/// @param near 
	/// @param far 
	/// @tparam T Value type used to build the matrix. Currently supported: half (not recommanded), float or double.
	/// @see gtc_matrix_transform
	template <typename T, precision P>
	GLM_FUNC_DECL detail::tmat4x4<T, P> frustum(
		T const & left,
		T const & right,
		T const & bottom,
		T const & top,
		T const & near,
		T const & far);

	/// Creates a matrix for a symetric perspective-view frustum.
	/// 
	/// @param fovy Expressed in radians if GLM_FORCE_RADIANS is define or degrees otherwise.
	/// @param aspect 
	/// @param near 
	/// @param far 
	/// @tparam T Value type used to build the matrix. Currently supported: half (not recommanded), float or double.
	/// @see gtc_matrix_transform
	template <typename T, precision P>
	GLM_FUNC_DECL detail::tmat4x4<T, P> perspective(
		T const & fovy,
		T const & aspect,
		T const & near,
		T const & far);

	/// Builds a perspective projection matrix based on a field of view.
	/// 
	/// @param fov Expressed in radians if GLM_FORCE_RADIANS is define or degrees otherwise.
	/// @param width 
	/// @param height 
	/// @param near 
	/// @param far 
	/// @tparam T Value type used to build the matrix. Currently supported: half (not recommanded), float or double.
	/// @see gtc_matrix_transform
	template <typename T, precision P>
	GLM_FUNC_DECL detail::tmat4x4<T, P> perspectiveFov(
		T const & fov,
		T const & width,
		T const & height,
		T const & near,
		T const & far);

	/// Creates a matrix for a symmetric perspective-view frustum with far plane at infinite.
	/// 
	/// @param fovy Expressed in radians if GLM_FORCE_RADIANS is define or degrees otherwise.
	/// @param aspect 
	/// @param near 
	/// @tparam T Value type used to build the matrix. Currently supported: half (not recommanded), float or double.
	/// @see gtc_matrix_transform
	template <typename T, precision P>
	GLM_FUNC_DECL detail::tmat4x4<T, P> infinitePerspective(
		T fovy, T aspect, T near);

	/// Creates a matrix for a symmetric perspective-view frustum with far plane at infinite for graphics hardware that doesn't support depth clamping.
	/// 
	/// @param fovy Expressed in radians if GLM_FORCE_RADIANS is define or degrees otherwise.
	/// @param aspect 
	/// @param near 
	/// @tparam T Value type used to build the matrix. Currently supported: half (not recommanded), float or double.
	/// @see gtc_matrix_transform
	template <typename T, precision P>
	GLM_FUNC_DECL detail::tmat4x4<T, P> tweakedInfinitePerspective(
		T fovy, T aspect, T near);

	/// Map the specified object coordinates (object.x, object.y, object.z) into window coordinates.
	/// 
	/// @param obj 
	/// @param model 
	/// @param proj
	/// @param viewport 
	/// @tparam T Native type used for the computation. Currently supported: half (not recommanded), float or double.
	/// @tparam U Currently supported: Floating-point types and integer types.
	/// @see gtc_matrix_transform
	template <typename T, typename U, precision P>
	GLM_FUNC_DECL detail::tvec3<T, P> project(
		detail::tvec3<T, P> const & obj,
		detail::tmat4x4<T, P> const & model,
		detail::tmat4x4<T, P> const & proj,
		detail::tvec4<U, P> const & viewport);

	/// Map the specified window coordinates (win.x, win.y, win.z) into object coordinates.
	///
	/// @param win
	/// @param model
	/// @param proj
	/// @param viewport
	/// @tparam T Native type used for the computation. Currently supported: half (not recommanded), float or double.
	/// @tparam U Currently supported: Floating-point types and integer types.
	/// @see gtc_matrix_transform
	template <typename T, typename U, precision P>
	GLM_FUNC_DECL detail::tvec3<T, P> unProject(
		detail::tvec3<T, P> const & win,
		detail::tmat4x4<T, P> const & model,
		detail::tmat4x4<T, P> const & proj,
		detail::tvec4<U, P> const & viewport);

	/// Define a picking region
	///
	/// @param center
	/// @param delta
	/// @param viewport
	/// @tparam T Native type used for the computation. Currently supported: half (not recommanded), float or double.
	/// @tparam U Currently supported: Floating-point types and integer types.
	/// @see gtc_matrix_transform
	template <typename T, precision P, typename U>
	GLM_FUNC_DECL detail::tmat4x4<T, P> pickMatrix(
		detail::tvec2<T, P> const & center,
		detail::tvec2<T, P> const & delta,
		detail::tvec4<U, P> const & viewport);

	/// Build a look at view matrix.
	///
	/// @param eye Position of the camera
	/// @param center Position where the camera is looking at
	/// @param up Normalized up vector, how the camera is oriented. Typically (0, 0, 1)
	/// @see gtc_matrix_transform
	/// @see - frustum(T const & left, T const & right, T const & bottom, T const & top, T const & nearVal, T const & farVal) frustum(T const & left, T const & right, T const & bottom, T const & top, T const & nearVal, T const & farVal)
	template <typename T, precision P>
	GLM_FUNC_DECL detail::tmat4x4<T, P> lookAt(
		detail::tvec3<T, P> const & eye,
		detail::tvec3<T, P> const & center,
		detail::tvec3<T, P> const & up);

	/// @}
}//namespace glm

#include "matrix_transform.inl"

#endif//GLM_GTC_matrix_transform
