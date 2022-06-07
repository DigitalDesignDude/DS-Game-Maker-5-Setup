#pragma once

#define __PAASSERT_CPP_START extern "C"{
#define __PAASSERT_CPP_END   }

#ifdef __cplusplus
__PAASSERT_CPP_START
#endif

/*! \addtogroup Debug
	@{
*/

/*! \def PA_Assert(c, m)
	\brief
		\~english Shows an error message if the condition is not true.
		\~french Afficher un erreur si la condition est faux.
	\~\param c
		\~english Condition, like MyVar < 128
		\~french Condition, comme MyVar < 128
	\~\param m
		\~english Error message
		\~french Message d'erreur
*/

#ifdef NDEBUG
#define PA_Assert(c,m) ((void)0)
#else
#define PA_Assert(c,m) ((c) ? ((void)0) : _PA_Assert(#c, m, __FILE__, __LINE__))
void _PA_Assert(const char* condition, const char* message, const char* file, int linen);
#endif

/*! @} */

#ifdef __cplusplus
__PAASSERT_CPP_END
#endif
