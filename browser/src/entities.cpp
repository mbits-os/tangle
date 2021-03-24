// Copyright (c) 2021 midnightBITS
// This code is licensed under MIT license (see LICENSE for details)
//
// clang-format off
//
// This file is generated, to recreate, call
// ninja -C build html-entities
// curl https://html.spec.whatwg.org/entities.json | build/bin/html-entities - browser/src/entities.cpp browser/tests/html_entities.cc

#include <tangle/browser/entities.hpp>

using namespace std::literals;

namespace tangle::browser {
#define CHAR_AT(NO) if (length > NO) { switch (name[NO]) {
#define CHAR(C) case C:
#define CHAR_END_NOBR() }}
#define CHAR_END() }} break;
#define ON_SUFFIX(NO, suffix, result) \
  if (name.substr(NO) == suffix ## sv) { return result; } \
  break
#define ON_LEAF_NOBR(NO, result) if (length == NO) return result
#define ON_LEAF(NO, result) ON_LEAF_NOBR(NO, result); break

char const* entity(std::string_view name) {
	auto const length = name.length();

	CHAR_AT(0)
		CHAR('A')
			CHAR_AT(1)
				CHAR('E') ON_SUFFIX(2, "lig", "\xc3\x86");  // &AElig;
				CHAR('M') ON_SUFFIX(2, "P", "&");  // &AMP;
				CHAR('a') ON_SUFFIX(2, "cute", "\xc3\x81");  // &Aacute;
				CHAR('b') ON_SUFFIX(2, "reve", "\xc4\x82");  // &Abreve;
				CHAR('c')
					CHAR_AT(2)
						CHAR('i') ON_SUFFIX(3, "rc", "\xc3\x82");  // &Acirc;
						CHAR('y') ON_LEAF(3, "\xd0\x90");  // &Acy;
					CHAR_END()
				CHAR('f') ON_SUFFIX(2, "r", "\xf0\x9d\x94\x84");  // &Afr;
				CHAR('g') ON_SUFFIX(2, "rave", "\xc3\x80");  // &Agrave;
				CHAR('l') ON_SUFFIX(2, "pha", "\xce\x91");  // &Alpha;
				CHAR('m') ON_SUFFIX(2, "acr", "\xc4\x80");  // &Amacr;
				CHAR('n') ON_SUFFIX(2, "d", "\xe2\xa9\x93");  // &And;
				CHAR('o')
					CHAR_AT(2)
						CHAR('g') ON_SUFFIX(3, "on", "\xc4\x84");  // &Aogon;
						CHAR('p') ON_SUFFIX(3, "f", "\xf0\x9d\x94\xb8");  // &Aopf;
					CHAR_END()
				CHAR('p') ON_SUFFIX(2, "plyFunction", "\xe2\x81\xa1");  // &ApplyFunction;
				CHAR('r') ON_SUFFIX(2, "ing", "\xc3\x85");  // &Aring;
				CHAR('s')
					CHAR_AT(2)
						CHAR('c') ON_SUFFIX(3, "r", "\xf0\x9d\x92\x9c");  // &Ascr;
						CHAR('s') ON_SUFFIX(3, "ign", "\xe2\x89\x94");  // &Assign;
					CHAR_END()
				CHAR('t') ON_SUFFIX(2, "ilde", "\xc3\x83");  // &Atilde;
				CHAR('u') ON_SUFFIX(2, "ml", "\xc3\x84");  // &Auml;
			CHAR_END()
		CHAR('B')
			CHAR_AT(1)
				CHAR('a')
					CHAR_AT(2)
						CHAR('c') ON_SUFFIX(3, "kslash", "\xe2\x88\x96");  // &Backslash;
						CHAR('r')
							CHAR_AT(3)
								CHAR('v') ON_LEAF(4, "\xe2\xab\xa7");  // &Barv;
								CHAR('w') ON_SUFFIX(4, "ed", "\xe2\x8c\x86");  // &Barwed;
							CHAR_END()
					CHAR_END()
				CHAR('c') ON_SUFFIX(2, "y", "\xd0\x91");  // &Bcy;
				CHAR('e')
					CHAR_AT(2)
						CHAR('c') ON_SUFFIX(3, "ause", "\xe2\x88\xb5");  // &Because;
						CHAR('r') ON_SUFFIX(3, "noullis", "\xe2\x84\xac");  // &Bernoullis;
						CHAR('t') ON_SUFFIX(3, "a", "\xce\x92");  // &Beta;
					CHAR_END()
				CHAR('f') ON_SUFFIX(2, "r", "\xf0\x9d\x94\x85");  // &Bfr;
				CHAR('o') ON_SUFFIX(2, "pf", "\xf0\x9d\x94\xb9");  // &Bopf;
				CHAR('r') ON_SUFFIX(2, "eve", "\xcb\x98");  // &Breve;
				CHAR('s') ON_SUFFIX(2, "cr", "\xe2\x84\xac");  // &Bscr;
				CHAR('u') ON_SUFFIX(2, "mpeq", "\xe2\x89\x8e");  // &Bumpeq;
			CHAR_END()
		CHAR('C')
			CHAR_AT(1)
				CHAR('H') ON_SUFFIX(2, "cy", "\xd0\xa7");  // &CHcy;
				CHAR('O') ON_SUFFIX(2, "PY", "\xc2\xa9");  // &COPY;
				CHAR('a')
					CHAR_AT(2)
						CHAR('c') ON_SUFFIX(3, "ute", "\xc4\x86");  // &Cacute;
						CHAR('p')
							ON_LEAF_NOBR(3, "\xe2\x8b\x92");  // &Cap;
							ON_SUFFIX(3, "italDifferentialD", "\xe2\x85\x85");  // &CapitalDifferentialD;
						CHAR('y') ON_SUFFIX(3, "leys", "\xe2\x84\xad");  // &Cayleys;
					CHAR_END()
				CHAR('c')
					CHAR_AT(2)
						CHAR('a') ON_SUFFIX(3, "ron", "\xc4\x8c");  // &Ccaron;
						CHAR('e') ON_SUFFIX(3, "dil", "\xc3\x87");  // &Ccedil;
						CHAR('i') ON_SUFFIX(3, "rc", "\xc4\x88");  // &Ccirc;
						CHAR('o') ON_SUFFIX(3, "nint", "\xe2\x88\xb0");  // &Cconint;
					CHAR_END()
				CHAR('d') ON_SUFFIX(2, "ot", "\xc4\x8a");  // &Cdot;
				CHAR('e')
					CHAR_AT(2)
						CHAR('d') ON_SUFFIX(3, "illa", "\xc2\xb8");  // &Cedilla;
						CHAR('n') ON_SUFFIX(3, "terDot", "\xc2\xb7");  // &CenterDot;
					CHAR_END()
				CHAR('f') ON_SUFFIX(2, "r", "\xe2\x84\xad");  // &Cfr;
				CHAR('h') ON_SUFFIX(2, "i", "\xce\xa7");  // &Chi;
				CHAR('i')
					CHAR_AT(2)
						CHAR('r')
							CHAR_AT(3)
								CHAR('c')
									CHAR_AT(4)
										CHAR('l')
											CHAR_AT(5)
												CHAR('e')
													CHAR_AT(6)
														CHAR('D') ON_SUFFIX(7, "ot", "\xe2\x8a\x99");  // &CircleDot;
														CHAR('M') ON_SUFFIX(7, "inus", "\xe2\x8a\x96");  // &CircleMinus;
														CHAR('P') ON_SUFFIX(7, "lus", "\xe2\x8a\x95");  // &CirclePlus;
														CHAR('T') ON_SUFFIX(7, "imes", "\xe2\x8a\x97");  // &CircleTimes;
													CHAR_END()
											CHAR_END()
									CHAR_END()
							CHAR_END()
					CHAR_END()
				CHAR('l')
					CHAR_AT(2)
						CHAR('o')
							CHAR_AT(3)
								CHAR('c') ON_SUFFIX(4, "kwiseContourIntegral", "\xe2\x88\xb2");  // &ClockwiseContourIntegral;
								CHAR('s')
									CHAR_AT(4)
										CHAR('e')
											CHAR_AT(5)
												CHAR('C')
													CHAR_AT(6)
														CHAR('u')
															CHAR_AT(7)
																CHAR('r')
																	CHAR_AT(8)
																		CHAR('l')
																			CHAR_AT(9)
																				CHAR('y')
																					CHAR_AT(10)
																						CHAR('D') ON_SUFFIX(11, "oubleQuote", "\xe2\x80\x9d");  // &CloseCurlyDoubleQuote;
																						CHAR('Q') ON_SUFFIX(11, "uote", "\xe2\x80\x99");  // &CloseCurlyQuote;
																					CHAR_END()
																			CHAR_END()
																	CHAR_END()
															CHAR_END()
													CHAR_END()
											CHAR_END()
									CHAR_END()
							CHAR_END()
					CHAR_END()
				CHAR('o')
					CHAR_AT(2)
						CHAR('l')
							CHAR_AT(3)
								CHAR('o')
									CHAR_AT(4)
										CHAR('n')
											ON_LEAF_NOBR(5, "\xe2\x88\xb7");  // &Colon;
											ON_SUFFIX(5, "e", "\xe2\xa9\xb4");  // &Colone;
									CHAR_END()
							CHAR_END()
						CHAR('n')
							CHAR_AT(3)
								CHAR('g') ON_SUFFIX(4, "ruent", "\xe2\x89\xa1");  // &Congruent;
								CHAR('i') ON_SUFFIX(4, "nt", "\xe2\x88\xaf");  // &Conint;
								CHAR('t') ON_SUFFIX(4, "ourIntegral", "\xe2\x88\xae");  // &ContourIntegral;
							CHAR_END()
						CHAR('p')
							CHAR_AT(3)
								CHAR('f') ON_LEAF(4, "\xe2\x84\x82");  // &Copf;
								CHAR('r') ON_SUFFIX(4, "oduct", "\xe2\x88\x90");  // &Coproduct;
							CHAR_END()
						CHAR('u') ON_SUFFIX(3, "nterClockwiseContourIntegral", "\xe2\x88\xb3");  // &CounterClockwiseContourIntegral;
					CHAR_END()
				CHAR('r') ON_SUFFIX(2, "oss", "\xe2\xa8\xaf");  // &Cross;
				CHAR('s') ON_SUFFIX(2, "cr", "\xf0\x9d\x92\x9e");  // &Cscr;
				CHAR('u')
					CHAR_AT(2)
						CHAR('p')
							ON_LEAF_NOBR(3, "\xe2\x8b\x93");  // &Cup;
							ON_SUFFIX(3, "Cap", "\xe2\x89\x8d");  // &CupCap;
					CHAR_END()
			CHAR_END()
		CHAR('D')
			CHAR_AT(1)
				CHAR('D')
					ON_LEAF_NOBR(2, "\xe2\x85\x85");  // &DD;
					ON_SUFFIX(2, "otrahd", "\xe2\xa4\x91");  // &DDotrahd;
				CHAR('J') ON_SUFFIX(2, "cy", "\xd0\x82");  // &DJcy;
				CHAR('S') ON_SUFFIX(2, "cy", "\xd0\x85");  // &DScy;
				CHAR('Z') ON_SUFFIX(2, "cy", "\xd0\x8f");  // &DZcy;
				CHAR('a')
					CHAR_AT(2)
						CHAR('g') ON_SUFFIX(3, "ger", "\xe2\x80\xa1");  // &Dagger;
						CHAR('r') ON_SUFFIX(3, "r", "\xe2\x86\xa1");  // &Darr;
						CHAR('s') ON_SUFFIX(3, "hv", "\xe2\xab\xa4");  // &Dashv;
					CHAR_END()
				CHAR('c')
					CHAR_AT(2)
						CHAR('a') ON_SUFFIX(3, "ron", "\xc4\x8e");  // &Dcaron;
						CHAR('y') ON_LEAF(3, "\xd0\x94");  // &Dcy;
					CHAR_END()
				CHAR('e')
					CHAR_AT(2)
						CHAR('l')
							ON_LEAF_NOBR(3, "\xe2\x88\x87");  // &Del;
							ON_SUFFIX(3, "ta", "\xce\x94");  // &Delta;
					CHAR_END()
				CHAR('f') ON_SUFFIX(2, "r", "\xf0\x9d\x94\x87");  // &Dfr;
				CHAR('i')
					CHAR_AT(2)
						CHAR('a')
							CHAR_AT(3)
								CHAR('c')
									CHAR_AT(4)
										CHAR('r')
											CHAR_AT(5)
												CHAR('i')
													CHAR_AT(6)
														CHAR('t')
															CHAR_AT(7)
																CHAR('i')
																	CHAR_AT(8)
																		CHAR('c')
																			CHAR_AT(9)
																				CHAR('a')
																					CHAR_AT(10)
																						CHAR('l')
																							CHAR_AT(11)
																								CHAR('A') ON_SUFFIX(12, "cute", "\xc2\xb4");  // &DiacriticalAcute;
																								CHAR('D')
																									CHAR_AT(12)
																										CHAR('o')
																											CHAR_AT(13)
																												CHAR('t') ON_LEAF(14, "\xcb\x99");  // &DiacriticalDot;
																												CHAR('u') ON_SUFFIX(14, "bleAcute", "\xcb\x9d");  // &DiacriticalDoubleAcute;
																											CHAR_END()
																									CHAR_END()
																								CHAR('G') ON_SUFFIX(12, "rave", "`");  // &DiacriticalGrave;
																								CHAR('T') ON_SUFFIX(12, "ilde", "\xcb\x9c");  // &DiacriticalTilde;
																							CHAR_END()
																					CHAR_END()
																			CHAR_END()
																	CHAR_END()
															CHAR_END()
													CHAR_END()
											CHAR_END()
									CHAR_END()
								CHAR('m') ON_SUFFIX(4, "ond", "\xe2\x8b\x84");  // &Diamond;
							CHAR_END()
						CHAR('f') ON_SUFFIX(3, "ferentialD", "\xe2\x85\x86");  // &DifferentialD;
					CHAR_END()
				CHAR('o')
					CHAR_AT(2)
						CHAR('p') ON_SUFFIX(3, "f", "\xf0\x9d\x94\xbb");  // &Dopf;
						CHAR('t')
							ON_LEAF_NOBR(3, "\xc2\xa8");  // &Dot;
							CHAR_AT(3)
								CHAR('D') ON_SUFFIX(4, "ot", "\xe2\x83\x9c");  // &DotDot;
								CHAR('E') ON_SUFFIX(4, "qual", "\xe2\x89\x90");  // &DotEqual;
							CHAR_END()
						CHAR('u')
							CHAR_AT(3)
								CHAR('b')
									CHAR_AT(4)
										CHAR('l')
											CHAR_AT(5)
												CHAR('e')
													CHAR_AT(6)
														CHAR('C') ON_SUFFIX(7, "ontourIntegral", "\xe2\x88\xaf");  // &DoubleContourIntegral;
														CHAR('D')
															CHAR_AT(7)
																CHAR('o')
																	CHAR_AT(8)
																		CHAR('t') ON_LEAF(9, "\xc2\xa8");  // &DoubleDot;
																		CHAR('w') ON_SUFFIX(9, "nArrow", "\xe2\x87\x93");  // &DoubleDownArrow;
																	CHAR_END()
															CHAR_END()
														CHAR('L')
															CHAR_AT(7)
																CHAR('e')
																	CHAR_AT(8)
																		CHAR('f')
																			CHAR_AT(9)
																				CHAR('t')
																					CHAR_AT(10)
																						CHAR('A') ON_SUFFIX(11, "rrow", "\xe2\x87\x90");  // &DoubleLeftArrow;
																						CHAR('R') ON_SUFFIX(11, "ightArrow", "\xe2\x87\x94");  // &DoubleLeftRightArrow;
																						CHAR('T') ON_SUFFIX(11, "ee", "\xe2\xab\xa4");  // &DoubleLeftTee;
																					CHAR_END()
																			CHAR_END()
																	CHAR_END()
																CHAR('o')
																	CHAR_AT(8)
																		CHAR('n')
																			CHAR_AT(9)
																				CHAR('g')
																					CHAR_AT(10)
																						CHAR('L')
																							CHAR_AT(11)
																								CHAR('e')
																									CHAR_AT(12)
																										CHAR('f')
																											CHAR_AT(13)
																												CHAR('t')
																													CHAR_AT(14)
																														CHAR('A') ON_SUFFIX(15, "rrow", "\xe2\x9f\xb8");  // &DoubleLongLeftArrow;
																														CHAR('R') ON_SUFFIX(15, "ightArrow", "\xe2\x9f\xba");  // &DoubleLongLeftRightArrow;
																													CHAR_END()
																											CHAR_END()
																									CHAR_END()
																							CHAR_END()
																						CHAR('R') ON_SUFFIX(11, "ightArrow", "\xe2\x9f\xb9");  // &DoubleLongRightArrow;
																					CHAR_END()
																			CHAR_END()
																	CHAR_END()
															CHAR_END()
														CHAR('R')
															CHAR_AT(7)
																CHAR('i')
																	CHAR_AT(8)
																		CHAR('g')
																			CHAR_AT(9)
																				CHAR('h')
																					CHAR_AT(10)
																						CHAR('t')
																							CHAR_AT(11)
																								CHAR('A') ON_SUFFIX(12, "rrow", "\xe2\x87\x92");  // &DoubleRightArrow;
																								CHAR('T') ON_SUFFIX(12, "ee", "\xe2\x8a\xa8");  // &DoubleRightTee;
																							CHAR_END()
																					CHAR_END()
																			CHAR_END()
																	CHAR_END()
															CHAR_END()
														CHAR('U')
															CHAR_AT(7)
																CHAR('p')
																	CHAR_AT(8)
																		CHAR('A') ON_SUFFIX(9, "rrow", "\xe2\x87\x91");  // &DoubleUpArrow;
																		CHAR('D') ON_SUFFIX(9, "ownArrow", "\xe2\x87\x95");  // &DoubleUpDownArrow;
																	CHAR_END()
															CHAR_END()
														CHAR('V') ON_SUFFIX(7, "erticalBar", "\xe2\x88\xa5");  // &DoubleVerticalBar;
													CHAR_END()
											CHAR_END()
									CHAR_END()
							CHAR_END()
						CHAR('w')
							CHAR_AT(3)
								CHAR('n')
									CHAR_AT(4)
										CHAR('A')
											CHAR_AT(5)
												CHAR('r')
													CHAR_AT(6)
														CHAR('r')
															CHAR_AT(7)
																CHAR('o')
																	CHAR_AT(8)
																		CHAR('w')
																			ON_LEAF_NOBR(9, "\xe2\x86\x93");  // &DownArrow;
																			CHAR_AT(9)
																				CHAR('B') ON_SUFFIX(10, "ar", "\xe2\xa4\x93");  // &DownArrowBar;
																				CHAR('U') ON_SUFFIX(10, "pArrow", "\xe2\x87\xb5");  // &DownArrowUpArrow;
																			CHAR_END()
																	CHAR_END()
															CHAR_END()
													CHAR_END()
											CHAR_END()
										CHAR('B') ON_SUFFIX(5, "reve", "\xcc\x91");  // &DownBreve;
										CHAR('L')
											CHAR_AT(5)
												CHAR('e')
													CHAR_AT(6)
														CHAR('f')
															CHAR_AT(7)
																CHAR('t')
																	CHAR_AT(8)
																		CHAR('R') ON_SUFFIX(9, "ightVector", "\xe2\xa5\x90");  // &DownLeftRightVector;
																		CHAR('T') ON_SUFFIX(9, "eeVector", "\xe2\xa5\x9e");  // &DownLeftTeeVector;
																		CHAR('V')
																			CHAR_AT(9)
																				CHAR('e')
																					CHAR_AT(10)
																						CHAR('c')
																							CHAR_AT(11)
																								CHAR('t')
																									CHAR_AT(12)
																										CHAR('o')
																											CHAR_AT(13)
																												CHAR('r')
																													ON_LEAF_NOBR(14, "\xe2\x86\xbd");  // &DownLeftVector;
																													ON_SUFFIX(14, "Bar", "\xe2\xa5\x96");  // &DownLeftVectorBar;
																											CHAR_END()
																									CHAR_END()
																							CHAR_END()
																					CHAR_END()
																			CHAR_END()
																	CHAR_END()
															CHAR_END()
													CHAR_END()
											CHAR_END()
										CHAR('R')
											CHAR_AT(5)
												CHAR('i')
													CHAR_AT(6)
														CHAR('g')
															CHAR_AT(7)
																CHAR('h')
																	CHAR_AT(8)
																		CHAR('t')
																			CHAR_AT(9)
																				CHAR('T') ON_SUFFIX(10, "eeVector", "\xe2\xa5\x9f");  // &DownRightTeeVector;
																				CHAR('V')
																					CHAR_AT(10)
																						CHAR('e')
																							CHAR_AT(11)
																								CHAR('c')
																									CHAR_AT(12)
																										CHAR('t')
																											CHAR_AT(13)
																												CHAR('o')
																													CHAR_AT(14)
																														CHAR('r')
																															ON_LEAF_NOBR(15, "\xe2\x87\x81");  // &DownRightVector;
																															ON_SUFFIX(15, "Bar", "\xe2\xa5\x97");  // &DownRightVectorBar;
																													CHAR_END()
																											CHAR_END()
																									CHAR_END()
																							CHAR_END()
																					CHAR_END()
																			CHAR_END()
																	CHAR_END()
															CHAR_END()
													CHAR_END()
											CHAR_END()
										CHAR('T')
											CHAR_AT(5)
												CHAR('e')
													CHAR_AT(6)
														CHAR('e')
															ON_LEAF_NOBR(7, "\xe2\x8a\xa4");  // &DownTee;
															ON_SUFFIX(7, "Arrow", "\xe2\x86\xa7");  // &DownTeeArrow;
													CHAR_END()
											CHAR_END()
										CHAR('a') ON_SUFFIX(5, "rrow", "\xe2\x87\x93");  // &Downarrow;
									CHAR_END()
							CHAR_END()
					CHAR_END()
				CHAR('s')
					CHAR_AT(2)
						CHAR('c') ON_SUFFIX(3, "r", "\xf0\x9d\x92\x9f");  // &Dscr;
						CHAR('t') ON_SUFFIX(3, "rok", "\xc4\x90");  // &Dstrok;
					CHAR_END()
			CHAR_END()
		CHAR('E')
			CHAR_AT(1)
				CHAR('N') ON_SUFFIX(2, "G", "\xc5\x8a");  // &ENG;
				CHAR('T') ON_SUFFIX(2, "H", "\xc3\x90");  // &ETH;
				CHAR('a') ON_SUFFIX(2, "cute", "\xc3\x89");  // &Eacute;
				CHAR('c')
					CHAR_AT(2)
						CHAR('a') ON_SUFFIX(3, "ron", "\xc4\x9a");  // &Ecaron;
						CHAR('i') ON_SUFFIX(3, "rc", "\xc3\x8a");  // &Ecirc;
						CHAR('y') ON_LEAF(3, "\xd0\xad");  // &Ecy;
					CHAR_END()
				CHAR('d') ON_SUFFIX(2, "ot", "\xc4\x96");  // &Edot;
				CHAR('f') ON_SUFFIX(2, "r", "\xf0\x9d\x94\x88");  // &Efr;
				CHAR('g') ON_SUFFIX(2, "rave", "\xc3\x88");  // &Egrave;
				CHAR('l') ON_SUFFIX(2, "ement", "\xe2\x88\x88");  // &Element;
				CHAR('m')
					CHAR_AT(2)
						CHAR('a') ON_SUFFIX(3, "cr", "\xc4\x92");  // &Emacr;
						CHAR('p')
							CHAR_AT(3)
								CHAR('t')
									CHAR_AT(4)
										CHAR('y')
											CHAR_AT(5)
												CHAR('S') ON_SUFFIX(6, "mallSquare", "\xe2\x97\xbb");  // &EmptySmallSquare;
												CHAR('V') ON_SUFFIX(6, "erySmallSquare", "\xe2\x96\xab");  // &EmptyVerySmallSquare;
											CHAR_END()
									CHAR_END()
							CHAR_END()
					CHAR_END()
				CHAR('o')
					CHAR_AT(2)
						CHAR('g') ON_SUFFIX(3, "on", "\xc4\x98");  // &Eogon;
						CHAR('p') ON_SUFFIX(3, "f", "\xf0\x9d\x94\xbc");  // &Eopf;
					CHAR_END()
				CHAR('p') ON_SUFFIX(2, "silon", "\xce\x95");  // &Epsilon;
				CHAR('q')
					CHAR_AT(2)
						CHAR('u')
							CHAR_AT(3)
								CHAR('a')
									CHAR_AT(4)
										CHAR('l')
											ON_LEAF_NOBR(5, "\xe2\xa9\xb5");  // &Equal;
											ON_SUFFIX(5, "Tilde", "\xe2\x89\x82");  // &EqualTilde;
									CHAR_END()
								CHAR('i') ON_SUFFIX(4, "librium", "\xe2\x87\x8c");  // &Equilibrium;
							CHAR_END()
					CHAR_END()
				CHAR('s')
					CHAR_AT(2)
						CHAR('c') ON_SUFFIX(3, "r", "\xe2\x84\xb0");  // &Escr;
						CHAR('i') ON_SUFFIX(3, "m", "\xe2\xa9\xb3");  // &Esim;
					CHAR_END()
				CHAR('t') ON_SUFFIX(2, "a", "\xce\x97");  // &Eta;
				CHAR('u') ON_SUFFIX(2, "ml", "\xc3\x8b");  // &Euml;
				CHAR('x')
					CHAR_AT(2)
						CHAR('i') ON_SUFFIX(3, "sts", "\xe2\x88\x83");  // &Exists;
						CHAR('p') ON_SUFFIX(3, "onentialE", "\xe2\x85\x87");  // &ExponentialE;
					CHAR_END()
			CHAR_END()
		CHAR('F')
			CHAR_AT(1)
				CHAR('c') ON_SUFFIX(2, "y", "\xd0\xa4");  // &Fcy;
				CHAR('f') ON_SUFFIX(2, "r", "\xf0\x9d\x94\x89");  // &Ffr;
				CHAR('i')
					CHAR_AT(2)
						CHAR('l')
							CHAR_AT(3)
								CHAR('l')
									CHAR_AT(4)
										CHAR('e')
											CHAR_AT(5)
												CHAR('d')
													CHAR_AT(6)
														CHAR('S') ON_SUFFIX(7, "mallSquare", "\xe2\x97\xbc");  // &FilledSmallSquare;
														CHAR('V') ON_SUFFIX(7, "erySmallSquare", "\xe2\x96\xaa");  // &FilledVerySmallSquare;
													CHAR_END()
											CHAR_END()
									CHAR_END()
							CHAR_END()
					CHAR_END()
				CHAR('o')
					CHAR_AT(2)
						CHAR('p') ON_SUFFIX(3, "f", "\xf0\x9d\x94\xbd");  // &Fopf;
						CHAR('r') ON_SUFFIX(3, "All", "\xe2\x88\x80");  // &ForAll;
						CHAR('u') ON_SUFFIX(3, "riertrf", "\xe2\x84\xb1");  // &Fouriertrf;
					CHAR_END()
				CHAR('s') ON_SUFFIX(2, "cr", "\xe2\x84\xb1");  // &Fscr;
			CHAR_END()
		CHAR('G')
			CHAR_AT(1)
				CHAR('J') ON_SUFFIX(2, "cy", "\xd0\x83");  // &GJcy;
				CHAR('T') ON_LEAF(2, ">");  // &GT;
				CHAR('a')
					CHAR_AT(2)
						CHAR('m')
							CHAR_AT(3)
								CHAR('m')
									CHAR_AT(4)
										CHAR('a')
											ON_LEAF_NOBR(5, "\xce\x93");  // &Gamma;
											ON_SUFFIX(5, "d", "\xcf\x9c");  // &Gammad;
									CHAR_END()
							CHAR_END()
					CHAR_END()
				CHAR('b') ON_SUFFIX(2, "reve", "\xc4\x9e");  // &Gbreve;
				CHAR('c')
					CHAR_AT(2)
						CHAR('e') ON_SUFFIX(3, "dil", "\xc4\xa2");  // &Gcedil;
						CHAR('i') ON_SUFFIX(3, "rc", "\xc4\x9c");  // &Gcirc;
						CHAR('y') ON_LEAF(3, "\xd0\x93");  // &Gcy;
					CHAR_END()
				CHAR('d') ON_SUFFIX(2, "ot", "\xc4\xa0");  // &Gdot;
				CHAR('f') ON_SUFFIX(2, "r", "\xf0\x9d\x94\x8a");  // &Gfr;
				CHAR('g') ON_LEAF(2, "\xe2\x8b\x99");  // &Gg;
				CHAR('o') ON_SUFFIX(2, "pf", "\xf0\x9d\x94\xbe");  // &Gopf;
				CHAR('r')
					CHAR_AT(2)
						CHAR('e')
							CHAR_AT(3)
								CHAR('a')
									CHAR_AT(4)
										CHAR('t')
											CHAR_AT(5)
												CHAR('e')
													CHAR_AT(6)
														CHAR('r')
															CHAR_AT(7)
																CHAR('E')
																	CHAR_AT(8)
																		CHAR('q')
																			CHAR_AT(9)
																				CHAR('u')
																					CHAR_AT(10)
																						CHAR('a')
																							CHAR_AT(11)
																								CHAR('l')
																									ON_LEAF_NOBR(12, "\xe2\x89\xa5");  // &GreaterEqual;
																									ON_SUFFIX(12, "Less", "\xe2\x8b\x9b");  // &GreaterEqualLess;
																							CHAR_END()
																					CHAR_END()
																			CHAR_END()
																	CHAR_END()
																CHAR('F') ON_SUFFIX(8, "ullEqual", "\xe2\x89\xa7");  // &GreaterFullEqual;
																CHAR('G') ON_SUFFIX(8, "reater", "\xe2\xaa\xa2");  // &GreaterGreater;
																CHAR('L') ON_SUFFIX(8, "ess", "\xe2\x89\xb7");  // &GreaterLess;
																CHAR('S') ON_SUFFIX(8, "lantEqual", "\xe2\xa9\xbe");  // &GreaterSlantEqual;
																CHAR('T') ON_SUFFIX(8, "ilde", "\xe2\x89\xb3");  // &GreaterTilde;
															CHAR_END()
													CHAR_END()
											CHAR_END()
									CHAR_END()
							CHAR_END()
					CHAR_END()
				CHAR('s') ON_SUFFIX(2, "cr", "\xf0\x9d\x92\xa2");  // &Gscr;
				CHAR('t') ON_LEAF(2, "\xe2\x89\xab");  // &Gt;
			CHAR_END()
		CHAR('H')
			CHAR_AT(1)
				CHAR('A') ON_SUFFIX(2, "RDcy", "\xd0\xaa");  // &HARDcy;
				CHAR('a')
					CHAR_AT(2)
						CHAR('c') ON_SUFFIX(3, "ek", "\xcb\x87");  // &Hacek;
						CHAR('t') ON_LEAF(3, "^");  // &Hat;
					CHAR_END()
				CHAR('c') ON_SUFFIX(2, "irc", "\xc4\xa4");  // &Hcirc;
				CHAR('f') ON_SUFFIX(2, "r", "\xe2\x84\x8c");  // &Hfr;
				CHAR('i') ON_SUFFIX(2, "lbertSpace", "\xe2\x84\x8b");  // &HilbertSpace;
				CHAR('o')
					CHAR_AT(2)
						CHAR('p') ON_SUFFIX(3, "f", "\xe2\x84\x8d");  // &Hopf;
						CHAR('r') ON_SUFFIX(3, "izontalLine", "\xe2\x94\x80");  // &HorizontalLine;
					CHAR_END()
				CHAR('s')
					CHAR_AT(2)
						CHAR('c') ON_SUFFIX(3, "r", "\xe2\x84\x8b");  // &Hscr;
						CHAR('t') ON_SUFFIX(3, "rok", "\xc4\xa6");  // &Hstrok;
					CHAR_END()
				CHAR('u')
					CHAR_AT(2)
						CHAR('m')
							CHAR_AT(3)
								CHAR('p')
									CHAR_AT(4)
										CHAR('D') ON_SUFFIX(5, "ownHump", "\xe2\x89\x8e");  // &HumpDownHump;
										CHAR('E') ON_SUFFIX(5, "qual", "\xe2\x89\x8f");  // &HumpEqual;
									CHAR_END()
							CHAR_END()
					CHAR_END()
			CHAR_END()
		CHAR('I')
			CHAR_AT(1)
				CHAR('E') ON_SUFFIX(2, "cy", "\xd0\x95");  // &IEcy;
				CHAR('J') ON_SUFFIX(2, "lig", "\xc4\xb2");  // &IJlig;
				CHAR('O') ON_SUFFIX(2, "cy", "\xd0\x81");  // &IOcy;
				CHAR('a') ON_SUFFIX(2, "cute", "\xc3\x8d");  // &Iacute;
				CHAR('c')
					CHAR_AT(2)
						CHAR('i') ON_SUFFIX(3, "rc", "\xc3\x8e");  // &Icirc;
						CHAR('y') ON_LEAF(3, "\xd0\x98");  // &Icy;
					CHAR_END()
				CHAR('d') ON_SUFFIX(2, "ot", "\xc4\xb0");  // &Idot;
				CHAR('f') ON_SUFFIX(2, "r", "\xe2\x84\x91");  // &Ifr;
				CHAR('g') ON_SUFFIX(2, "rave", "\xc3\x8c");  // &Igrave;
				CHAR('m')
					ON_LEAF_NOBR(2, "\xe2\x84\x91");  // &Im;
					CHAR_AT(2)
						CHAR('a')
							CHAR_AT(3)
								CHAR('c') ON_SUFFIX(4, "r", "\xc4\xaa");  // &Imacr;
								CHAR('g') ON_SUFFIX(4, "inaryI", "\xe2\x85\x88");  // &ImaginaryI;
							CHAR_END()
						CHAR('p') ON_SUFFIX(3, "lies", "\xe2\x87\x92");  // &Implies;
					CHAR_END()
				CHAR('n')
					CHAR_AT(2)
						CHAR('t')
							ON_LEAF_NOBR(3, "\xe2\x88\xac");  // &Int;
							CHAR_AT(3)
								CHAR('e')
									CHAR_AT(4)
										CHAR('g') ON_SUFFIX(5, "ral", "\xe2\x88\xab");  // &Integral;
										CHAR('r') ON_SUFFIX(5, "section", "\xe2\x8b\x82");  // &Intersection;
									CHAR_END()
							CHAR_END()
						CHAR('v')
							CHAR_AT(3)
								CHAR('i')
									CHAR_AT(4)
										CHAR('s')
											CHAR_AT(5)
												CHAR('i')
													CHAR_AT(6)
														CHAR('b')
															CHAR_AT(7)
																CHAR('l')
																	CHAR_AT(8)
																		CHAR('e')
																			CHAR_AT(9)
																				CHAR('C') ON_SUFFIX(10, "omma", "\xe2\x81\xa3");  // &InvisibleComma;
																				CHAR('T') ON_SUFFIX(10, "imes", "\xe2\x81\xa2");  // &InvisibleTimes;
																			CHAR_END()
																	CHAR_END()
															CHAR_END()
													CHAR_END()
											CHAR_END()
									CHAR_END()
							CHAR_END()
					CHAR_END()
				CHAR('o')
					CHAR_AT(2)
						CHAR('g') ON_SUFFIX(3, "on", "\xc4\xae");  // &Iogon;
						CHAR('p') ON_SUFFIX(3, "f", "\xf0\x9d\x95\x80");  // &Iopf;
						CHAR('t') ON_SUFFIX(3, "a", "\xce\x99");  // &Iota;
					CHAR_END()
				CHAR('s') ON_SUFFIX(2, "cr", "\xe2\x84\x90");  // &Iscr;
				CHAR('t') ON_SUFFIX(2, "ilde", "\xc4\xa8");  // &Itilde;
				CHAR('u')
					CHAR_AT(2)
						CHAR('k') ON_SUFFIX(3, "cy", "\xd0\x86");  // &Iukcy;
						CHAR('m') ON_SUFFIX(3, "l", "\xc3\x8f");  // &Iuml;
					CHAR_END()
			CHAR_END()
		CHAR('J')
			CHAR_AT(1)
				CHAR('c')
					CHAR_AT(2)
						CHAR('i') ON_SUFFIX(3, "rc", "\xc4\xb4");  // &Jcirc;
						CHAR('y') ON_LEAF(3, "\xd0\x99");  // &Jcy;
					CHAR_END()
				CHAR('f') ON_SUFFIX(2, "r", "\xf0\x9d\x94\x8d");  // &Jfr;
				CHAR('o') ON_SUFFIX(2, "pf", "\xf0\x9d\x95\x81");  // &Jopf;
				CHAR('s')
					CHAR_AT(2)
						CHAR('c') ON_SUFFIX(3, "r", "\xf0\x9d\x92\xa5");  // &Jscr;
						CHAR('e') ON_SUFFIX(3, "rcy", "\xd0\x88");  // &Jsercy;
					CHAR_END()
				CHAR('u') ON_SUFFIX(2, "kcy", "\xd0\x84");  // &Jukcy;
			CHAR_END()
		CHAR('K')
			CHAR_AT(1)
				CHAR('H') ON_SUFFIX(2, "cy", "\xd0\xa5");  // &KHcy;
				CHAR('J') ON_SUFFIX(2, "cy", "\xd0\x8c");  // &KJcy;
				CHAR('a') ON_SUFFIX(2, "ppa", "\xce\x9a");  // &Kappa;
				CHAR('c')
					CHAR_AT(2)
						CHAR('e') ON_SUFFIX(3, "dil", "\xc4\xb6");  // &Kcedil;
						CHAR('y') ON_LEAF(3, "\xd0\x9a");  // &Kcy;
					CHAR_END()
				CHAR('f') ON_SUFFIX(2, "r", "\xf0\x9d\x94\x8e");  // &Kfr;
				CHAR('o') ON_SUFFIX(2, "pf", "\xf0\x9d\x95\x82");  // &Kopf;
				CHAR('s') ON_SUFFIX(2, "cr", "\xf0\x9d\x92\xa6");  // &Kscr;
			CHAR_END()
		CHAR('L')
			CHAR_AT(1)
				CHAR('J') ON_SUFFIX(2, "cy", "\xd0\x89");  // &LJcy;
				CHAR('T') ON_LEAF(2, "<");  // &LT;
				CHAR('a')
					CHAR_AT(2)
						CHAR('c') ON_SUFFIX(3, "ute", "\xc4\xb9");  // &Lacute;
						CHAR('m') ON_SUFFIX(3, "bda", "\xce\x9b");  // &Lambda;
						CHAR('n') ON_SUFFIX(3, "g", "\xe2\x9f\xaa");  // &Lang;
						CHAR('p') ON_SUFFIX(3, "lacetrf", "\xe2\x84\x92");  // &Laplacetrf;
						CHAR('r') ON_SUFFIX(3, "r", "\xe2\x86\x9e");  // &Larr;
					CHAR_END()
				CHAR('c')
					CHAR_AT(2)
						CHAR('a') ON_SUFFIX(3, "ron", "\xc4\xbd");  // &Lcaron;
						CHAR('e') ON_SUFFIX(3, "dil", "\xc4\xbb");  // &Lcedil;
						CHAR('y') ON_LEAF(3, "\xd0\x9b");  // &Lcy;
					CHAR_END()
				CHAR('e')
					CHAR_AT(2)
						CHAR('f')
							CHAR_AT(3)
								CHAR('t')
									CHAR_AT(4)
										CHAR('A')
											CHAR_AT(5)
												CHAR('n') ON_SUFFIX(6, "gleBracket", "\xe2\x9f\xa8");  // &LeftAngleBracket;
												CHAR('r')
													CHAR_AT(6)
														CHAR('r')
															CHAR_AT(7)
																CHAR('o')
																	CHAR_AT(8)
																		CHAR('w')
																			ON_LEAF_NOBR(9, "\xe2\x86\x90");  // &LeftArrow;
																			CHAR_AT(9)
																				CHAR('B') ON_SUFFIX(10, "ar", "\xe2\x87\xa4");  // &LeftArrowBar;
																				CHAR('R') ON_SUFFIX(10, "ightArrow", "\xe2\x87\x86");  // &LeftArrowRightArrow;
																			CHAR_END()
																	CHAR_END()
															CHAR_END()
													CHAR_END()
											CHAR_END()
										CHAR('C') ON_SUFFIX(5, "eiling", "\xe2\x8c\x88");  // &LeftCeiling;
										CHAR('D')
											CHAR_AT(5)
												CHAR('o')
													CHAR_AT(6)
														CHAR('u') ON_SUFFIX(7, "bleBracket", "\xe2\x9f\xa6");  // &LeftDoubleBracket;
														CHAR('w')
															CHAR_AT(7)
																CHAR('n')
																	CHAR_AT(8)
																		CHAR('T') ON_SUFFIX(9, "eeVector", "\xe2\xa5\xa1");  // &LeftDownTeeVector;
																		CHAR('V')
																			CHAR_AT(9)
																				CHAR('e')
																					CHAR_AT(10)
																						CHAR('c')
																							CHAR_AT(11)
																								CHAR('t')
																									CHAR_AT(12)
																										CHAR('o')
																											CHAR_AT(13)
																												CHAR('r')
																													ON_LEAF_NOBR(14, "\xe2\x87\x83");  // &LeftDownVector;
																													ON_SUFFIX(14, "Bar", "\xe2\xa5\x99");  // &LeftDownVectorBar;
																											CHAR_END()
																									CHAR_END()
																							CHAR_END()
																					CHAR_END()
																			CHAR_END()
																	CHAR_END()
															CHAR_END()
													CHAR_END()
											CHAR_END()
										CHAR('F') ON_SUFFIX(5, "loor", "\xe2\x8c\x8a");  // &LeftFloor;
										CHAR('R')
											CHAR_AT(5)
												CHAR('i')
													CHAR_AT(6)
														CHAR('g')
															CHAR_AT(7)
																CHAR('h')
																	CHAR_AT(8)
																		CHAR('t')
																			CHAR_AT(9)
																				CHAR('A') ON_SUFFIX(10, "rrow", "\xe2\x86\x94");  // &LeftRightArrow;
																				CHAR('V') ON_SUFFIX(10, "ector", "\xe2\xa5\x8e");  // &LeftRightVector;
																			CHAR_END()
																	CHAR_END()
															CHAR_END()
													CHAR_END()
											CHAR_END()
										CHAR('T')
											CHAR_AT(5)
												CHAR('e')
													CHAR_AT(6)
														CHAR('e')
															ON_LEAF_NOBR(7, "\xe2\x8a\xa3");  // &LeftTee;
															CHAR_AT(7)
																CHAR('A') ON_SUFFIX(8, "rrow", "\xe2\x86\xa4");  // &LeftTeeArrow;
																CHAR('V') ON_SUFFIX(8, "ector", "\xe2\xa5\x9a");  // &LeftTeeVector;
															CHAR_END()
													CHAR_END()
												CHAR('r')
													CHAR_AT(6)
														CHAR('i')
															CHAR_AT(7)
																CHAR('a')
																	CHAR_AT(8)
																		CHAR('n')
																			CHAR_AT(9)
																				CHAR('g')
																					CHAR_AT(10)
																						CHAR('l')
																							CHAR_AT(11)
																								CHAR('e')
																									ON_LEAF_NOBR(12, "\xe2\x8a\xb2");  // &LeftTriangle;
																									CHAR_AT(12)
																										CHAR('B') ON_SUFFIX(13, "ar", "\xe2\xa7\x8f");  // &LeftTriangleBar;
																										CHAR('E') ON_SUFFIX(13, "qual", "\xe2\x8a\xb4");  // &LeftTriangleEqual;
																									CHAR_END()
																							CHAR_END()
																					CHAR_END()
																			CHAR_END()
																	CHAR_END()
															CHAR_END()
													CHAR_END()
											CHAR_END()
										CHAR('U')
											CHAR_AT(5)
												CHAR('p')
													CHAR_AT(6)
														CHAR('D') ON_SUFFIX(7, "ownVector", "\xe2\xa5\x91");  // &LeftUpDownVector;
														CHAR('T') ON_SUFFIX(7, "eeVector", "\xe2\xa5\xa0");  // &LeftUpTeeVector;
														CHAR('V')
															CHAR_AT(7)
																CHAR('e')
																	CHAR_AT(8)
																		CHAR('c')
																			CHAR_AT(9)
																				CHAR('t')
																					CHAR_AT(10)
																						CHAR('o')
																							CHAR_AT(11)
																								CHAR('r')
																									ON_LEAF_NOBR(12, "\xe2\x86\xbf");  // &LeftUpVector;
																									ON_SUFFIX(12, "Bar", "\xe2\xa5\x98");  // &LeftUpVectorBar;
																							CHAR_END()
																					CHAR_END()
																			CHAR_END()
																	CHAR_END()
															CHAR_END()
													CHAR_END()
											CHAR_END()
										CHAR('V')
											CHAR_AT(5)
												CHAR('e')
													CHAR_AT(6)
														CHAR('c')
															CHAR_AT(7)
																CHAR('t')
																	CHAR_AT(8)
																		CHAR('o')
																			CHAR_AT(9)
																				CHAR('r')
																					ON_LEAF_NOBR(10, "\xe2\x86\xbc");  // &LeftVector;
																					ON_SUFFIX(10, "Bar", "\xe2\xa5\x92");  // &LeftVectorBar;
																			CHAR_END()
																	CHAR_END()
															CHAR_END()
													CHAR_END()
											CHAR_END()
										CHAR('a') ON_SUFFIX(5, "rrow", "\xe2\x87\x90");  // &Leftarrow;
										CHAR('r') ON_SUFFIX(5, "ightarrow", "\xe2\x87\x94");  // &Leftrightarrow;
									CHAR_END()
							CHAR_END()
						CHAR('s')
							CHAR_AT(3)
								CHAR('s')
									CHAR_AT(4)
										CHAR('E') ON_SUFFIX(5, "qualGreater", "\xe2\x8b\x9a");  // &LessEqualGreater;
										CHAR('F') ON_SUFFIX(5, "ullEqual", "\xe2\x89\xa6");  // &LessFullEqual;
										CHAR('G') ON_SUFFIX(5, "reater", "\xe2\x89\xb6");  // &LessGreater;
										CHAR('L') ON_SUFFIX(5, "ess", "\xe2\xaa\xa1");  // &LessLess;
										CHAR('S') ON_SUFFIX(5, "lantEqual", "\xe2\xa9\xbd");  // &LessSlantEqual;
										CHAR('T') ON_SUFFIX(5, "ilde", "\xe2\x89\xb2");  // &LessTilde;
									CHAR_END()
							CHAR_END()
					CHAR_END()
				CHAR('f') ON_SUFFIX(2, "r", "\xf0\x9d\x94\x8f");  // &Lfr;
				CHAR('l')
					ON_LEAF_NOBR(2, "\xe2\x8b\x98");  // &Ll;
					ON_SUFFIX(2, "eftarrow", "\xe2\x87\x9a");  // &Lleftarrow;
				CHAR('m') ON_SUFFIX(2, "idot", "\xc4\xbf");  // &Lmidot;
				CHAR('o')
					CHAR_AT(2)
						CHAR('n')
							CHAR_AT(3)
								CHAR('g')
									CHAR_AT(4)
										CHAR('L')
											CHAR_AT(5)
												CHAR('e')
													CHAR_AT(6)
														CHAR('f')
															CHAR_AT(7)
																CHAR('t')
																	CHAR_AT(8)
																		CHAR('A') ON_SUFFIX(9, "rrow", "\xe2\x9f\xb5");  // &LongLeftArrow;
																		CHAR('R') ON_SUFFIX(9, "ightArrow", "\xe2\x9f\xb7");  // &LongLeftRightArrow;
																	CHAR_END()
															CHAR_END()
													CHAR_END()
											CHAR_END()
										CHAR('R') ON_SUFFIX(5, "ightArrow", "\xe2\x9f\xb6");  // &LongRightArrow;
										CHAR('l')
											CHAR_AT(5)
												CHAR('e')
													CHAR_AT(6)
														CHAR('f')
															CHAR_AT(7)
																CHAR('t')
																	CHAR_AT(8)
																		CHAR('a') ON_SUFFIX(9, "rrow", "\xe2\x9f\xb8");  // &Longleftarrow;
																		CHAR('r') ON_SUFFIX(9, "ightarrow", "\xe2\x9f\xba");  // &Longleftrightarrow;
																	CHAR_END()
															CHAR_END()
													CHAR_END()
											CHAR_END()
										CHAR('r') ON_SUFFIX(5, "ightarrow", "\xe2\x9f\xb9");  // &Longrightarrow;
									CHAR_END()
							CHAR_END()
						CHAR('p') ON_SUFFIX(3, "f", "\xf0\x9d\x95\x83");  // &Lopf;
						CHAR('w')
							CHAR_AT(3)
								CHAR('e')
									CHAR_AT(4)
										CHAR('r')
											CHAR_AT(5)
												CHAR('L') ON_SUFFIX(6, "eftArrow", "\xe2\x86\x99");  // &LowerLeftArrow;
												CHAR('R') ON_SUFFIX(6, "ightArrow", "\xe2\x86\x98");  // &LowerRightArrow;
											CHAR_END()
									CHAR_END()
							CHAR_END()
					CHAR_END()
				CHAR('s')
					CHAR_AT(2)
						CHAR('c') ON_SUFFIX(3, "r", "\xe2\x84\x92");  // &Lscr;
						CHAR('h') ON_LEAF(3, "\xe2\x86\xb0");  // &Lsh;
						CHAR('t') ON_SUFFIX(3, "rok", "\xc5\x81");  // &Lstrok;
					CHAR_END()
				CHAR('t') ON_LEAF(2, "\xe2\x89\xaa");  // &Lt;
			CHAR_END()
		CHAR('M')
			CHAR_AT(1)
				CHAR('a') ON_SUFFIX(2, "p", "\xe2\xa4\x85");  // &Map;
				CHAR('c') ON_SUFFIX(2, "y", "\xd0\x9c");  // &Mcy;
				CHAR('e')
					CHAR_AT(2)
						CHAR('d') ON_SUFFIX(3, "iumSpace", "\xe2\x81\x9f");  // &MediumSpace;
						CHAR('l') ON_SUFFIX(3, "lintrf", "\xe2\x84\xb3");  // &Mellintrf;
					CHAR_END()
				CHAR('f') ON_SUFFIX(2, "r", "\xf0\x9d\x94\x90");  // &Mfr;
				CHAR('i') ON_SUFFIX(2, "nusPlus", "\xe2\x88\x93");  // &MinusPlus;
				CHAR('o') ON_SUFFIX(2, "pf", "\xf0\x9d\x95\x84");  // &Mopf;
				CHAR('s') ON_SUFFIX(2, "cr", "\xe2\x84\xb3");  // &Mscr;
				CHAR('u') ON_LEAF(2, "\xce\x9c");  // &Mu;
			CHAR_END()
		CHAR('N')
			CHAR_AT(1)
				CHAR('J') ON_SUFFIX(2, "cy", "\xd0\x8a");  // &NJcy;
				CHAR('a') ON_SUFFIX(2, "cute", "\xc5\x83");  // &Nacute;
				CHAR('c')
					CHAR_AT(2)
						CHAR('a') ON_SUFFIX(3, "ron", "\xc5\x87");  // &Ncaron;
						CHAR('e') ON_SUFFIX(3, "dil", "\xc5\x85");  // &Ncedil;
						CHAR('y') ON_LEAF(3, "\xd0\x9d");  // &Ncy;
					CHAR_END()
				CHAR('e')
					CHAR_AT(2)
						CHAR('g')
							CHAR_AT(3)
								CHAR('a')
									CHAR_AT(4)
										CHAR('t')
											CHAR_AT(5)
												CHAR('i')
													CHAR_AT(6)
														CHAR('v')
															CHAR_AT(7)
																CHAR('e')
																	CHAR_AT(8)
																		CHAR('M') ON_SUFFIX(9, "ediumSpace", "\xe2\x80\x8b");  // &NegativeMediumSpace;
																		CHAR('T')
																			CHAR_AT(9)
																				CHAR('h')
																					CHAR_AT(10)
																						CHAR('i')
																							CHAR_AT(11)
																								CHAR('c') ON_SUFFIX(12, "kSpace", "\xe2\x80\x8b");  // &NegativeThickSpace;
																								CHAR('n') ON_SUFFIX(12, "Space", "\xe2\x80\x8b");  // &NegativeThinSpace;
																							CHAR_END()
																					CHAR_END()
																			CHAR_END()
																		CHAR('V') ON_SUFFIX(9, "eryThinSpace", "\xe2\x80\x8b");  // &NegativeVeryThinSpace;
																	CHAR_END()
															CHAR_END()
													CHAR_END()
											CHAR_END()
									CHAR_END()
							CHAR_END()
						CHAR('s')
							CHAR_AT(3)
								CHAR('t')
									CHAR_AT(4)
										CHAR('e')
											CHAR_AT(5)
												CHAR('d')
													CHAR_AT(6)
														CHAR('G') ON_SUFFIX(7, "reaterGreater", "\xe2\x89\xab");  // &NestedGreaterGreater;
														CHAR('L') ON_SUFFIX(7, "essLess", "\xe2\x89\xaa");  // &NestedLessLess;
													CHAR_END()
											CHAR_END()
									CHAR_END()
							CHAR_END()
						CHAR('w') ON_SUFFIX(3, "Line", "\x0a");  // &NewLine;
					CHAR_END()
				CHAR('f') ON_SUFFIX(2, "r", "\xf0\x9d\x94\x91");  // &Nfr;
				CHAR('o')
					CHAR_AT(2)
						CHAR('B') ON_SUFFIX(3, "reak", "\xe2\x81\xa0");  // &NoBreak;
						CHAR('n') ON_SUFFIX(3, "BreakingSpace", "\xc2\xa0");  // &NonBreakingSpace;
						CHAR('p') ON_SUFFIX(3, "f", "\xe2\x84\x95");  // &Nopf;
						CHAR('t')
							ON_LEAF_NOBR(3, "\xe2\xab\xac");  // &Not;
							CHAR_AT(3)
								CHAR('C')
									CHAR_AT(4)
										CHAR('o') ON_SUFFIX(5, "ngruent", "\xe2\x89\xa2");  // &NotCongruent;
										CHAR('u') ON_SUFFIX(5, "pCap", "\xe2\x89\xad");  // &NotCupCap;
									CHAR_END()
								CHAR('D') ON_SUFFIX(4, "oubleVerticalBar", "\xe2\x88\xa6");  // &NotDoubleVerticalBar;
								CHAR('E')
									CHAR_AT(4)
										CHAR('l') ON_SUFFIX(5, "ement", "\xe2\x88\x89");  // &NotElement;
										CHAR('q')
											CHAR_AT(5)
												CHAR('u')
													CHAR_AT(6)
														CHAR('a')
															CHAR_AT(7)
																CHAR('l')
																	ON_LEAF_NOBR(8, "\xe2\x89\xa0");  // &NotEqual;
																	ON_SUFFIX(8, "Tilde", "\xe2\x89\x82\xcc\xb8");  // &NotEqualTilde;
															CHAR_END()
													CHAR_END()
											CHAR_END()
										CHAR('x') ON_SUFFIX(5, "ists", "\xe2\x88\x84");  // &NotExists;
									CHAR_END()
								CHAR('G')
									CHAR_AT(4)
										CHAR('r')
											CHAR_AT(5)
												CHAR('e')
													CHAR_AT(6)
														CHAR('a')
															CHAR_AT(7)
																CHAR('t')
																	CHAR_AT(8)
																		CHAR('e')
																			CHAR_AT(9)
																				CHAR('r')
																					ON_LEAF_NOBR(10, "\xe2\x89\xaf");  // &NotGreater;
																					CHAR_AT(10)
																						CHAR('E') ON_SUFFIX(11, "qual", "\xe2\x89\xb1");  // &NotGreaterEqual;
																						CHAR('F') ON_SUFFIX(11, "ullEqual", "\xe2\x89\xa7\xcc\xb8");  // &NotGreaterFullEqual;
																						CHAR('G') ON_SUFFIX(11, "reater", "\xe2\x89\xab\xcc\xb8");  // &NotGreaterGreater;
																						CHAR('L') ON_SUFFIX(11, "ess", "\xe2\x89\xb9");  // &NotGreaterLess;
																						CHAR('S') ON_SUFFIX(11, "lantEqual", "\xe2\xa9\xbe\xcc\xb8");  // &NotGreaterSlantEqual;
																						CHAR('T') ON_SUFFIX(11, "ilde", "\xe2\x89\xb5");  // &NotGreaterTilde;
																					CHAR_END()
																			CHAR_END()
																	CHAR_END()
															CHAR_END()
													CHAR_END()
											CHAR_END()
									CHAR_END()
								CHAR('H')
									CHAR_AT(4)
										CHAR('u')
											CHAR_AT(5)
												CHAR('m')
													CHAR_AT(6)
														CHAR('p')
															CHAR_AT(7)
																CHAR('D') ON_SUFFIX(8, "ownHump", "\xe2\x89\x8e\xcc\xb8");  // &NotHumpDownHump;
																CHAR('E') ON_SUFFIX(8, "qual", "\xe2\x89\x8f\xcc\xb8");  // &NotHumpEqual;
															CHAR_END()
													CHAR_END()
											CHAR_END()
									CHAR_END()
								CHAR('L')
									CHAR_AT(4)
										CHAR('e')
											CHAR_AT(5)
												CHAR('f')
													CHAR_AT(6)
														CHAR('t')
															CHAR_AT(7)
																CHAR('T')
																	CHAR_AT(8)
																		CHAR('r')
																			CHAR_AT(9)
																				CHAR('i')
																					CHAR_AT(10)
																						CHAR('a')
																							CHAR_AT(11)
																								CHAR('n')
																									CHAR_AT(12)
																										CHAR('g')
																											CHAR_AT(13)
																												CHAR('l')
																													CHAR_AT(14)
																														CHAR('e')
																															ON_LEAF_NOBR(15, "\xe2\x8b\xaa");  // &NotLeftTriangle;
																															CHAR_AT(15)
																																CHAR('B') ON_SUFFIX(16, "ar", "\xe2\xa7\x8f\xcc\xb8");  // &NotLeftTriangleBar;
																																CHAR('E') ON_SUFFIX(16, "qual", "\xe2\x8b\xac");  // &NotLeftTriangleEqual;
																															CHAR_END()
																													CHAR_END()
																											CHAR_END()
																									CHAR_END()
																							CHAR_END()
																					CHAR_END()
																			CHAR_END()
																	CHAR_END()
															CHAR_END()
													CHAR_END()
												CHAR('s')
													CHAR_AT(6)
														CHAR('s')
															ON_LEAF_NOBR(7, "\xe2\x89\xae");  // &NotLess;
															CHAR_AT(7)
																CHAR('E') ON_SUFFIX(8, "qual", "\xe2\x89\xb0");  // &NotLessEqual;
																CHAR('G') ON_SUFFIX(8, "reater", "\xe2\x89\xb8");  // &NotLessGreater;
																CHAR('L') ON_SUFFIX(8, "ess", "\xe2\x89\xaa\xcc\xb8");  // &NotLessLess;
																CHAR('S') ON_SUFFIX(8, "lantEqual", "\xe2\xa9\xbd\xcc\xb8");  // &NotLessSlantEqual;
																CHAR('T') ON_SUFFIX(8, "ilde", "\xe2\x89\xb4");  // &NotLessTilde;
															CHAR_END()
													CHAR_END()
											CHAR_END()
									CHAR_END()
								CHAR('N')
									CHAR_AT(4)
										CHAR('e')
											CHAR_AT(5)
												CHAR('s')
													CHAR_AT(6)
														CHAR('t')
															CHAR_AT(7)
																CHAR('e')
																	CHAR_AT(8)
																		CHAR('d')
																			CHAR_AT(9)
																				CHAR('G') ON_SUFFIX(10, "reaterGreater", "\xe2\xaa\xa2\xcc\xb8");  // &NotNestedGreaterGreater;
																				CHAR('L') ON_SUFFIX(10, "essLess", "\xe2\xaa\xa1\xcc\xb8");  // &NotNestedLessLess;
																			CHAR_END()
																	CHAR_END()
															CHAR_END()
													CHAR_END()
											CHAR_END()
									CHAR_END()
								CHAR('P')
									CHAR_AT(4)
										CHAR('r')
											CHAR_AT(5)
												CHAR('e')
													CHAR_AT(6)
														CHAR('c')
															CHAR_AT(7)
																CHAR('e')
																	CHAR_AT(8)
																		CHAR('d')
																			CHAR_AT(9)
																				CHAR('e')
																					CHAR_AT(10)
																						CHAR('s')
																							ON_LEAF_NOBR(11, "\xe2\x8a\x80");  // &NotPrecedes;
																							CHAR_AT(11)
																								CHAR('E') ON_SUFFIX(12, "qual", "\xe2\xaa\xaf\xcc\xb8");  // &NotPrecedesEqual;
																								CHAR('S') ON_SUFFIX(12, "lantEqual", "\xe2\x8b\xa0");  // &NotPrecedesSlantEqual;
																							CHAR_END()
																					CHAR_END()
																			CHAR_END()
																	CHAR_END()
															CHAR_END()
													CHAR_END()
											CHAR_END()
									CHAR_END()
								CHAR('R')
									CHAR_AT(4)
										CHAR('e') ON_SUFFIX(5, "verseElement", "\xe2\x88\x8c");  // &NotReverseElement;
										CHAR('i')
											CHAR_AT(5)
												CHAR('g')
													CHAR_AT(6)
														CHAR('h')
															CHAR_AT(7)
																CHAR('t')
																	CHAR_AT(8)
																		CHAR('T')
																			CHAR_AT(9)
																				CHAR('r')
																					CHAR_AT(10)
																						CHAR('i')
																							CHAR_AT(11)
																								CHAR('a')
																									CHAR_AT(12)
																										CHAR('n')
																											CHAR_AT(13)
																												CHAR('g')
																													CHAR_AT(14)
																														CHAR('l')
																															CHAR_AT(15)
																																CHAR('e')
																																	ON_LEAF_NOBR(16, "\xe2\x8b\xab");  // &NotRightTriangle;
																																	CHAR_AT(16)
																																		CHAR('B') ON_SUFFIX(17, "ar", "\xe2\xa7\x90\xcc\xb8");  // &NotRightTriangleBar;
																																		CHAR('E') ON_SUFFIX(17, "qual", "\xe2\x8b\xad");  // &NotRightTriangleEqual;
																																	CHAR_END()
																															CHAR_END()
																													CHAR_END()
																											CHAR_END()
																									CHAR_END()
																							CHAR_END()
																					CHAR_END()
																			CHAR_END()
																	CHAR_END()
															CHAR_END()
													CHAR_END()
											CHAR_END()
									CHAR_END()
								CHAR('S')
									CHAR_AT(4)
										CHAR('q')
											CHAR_AT(5)
												CHAR('u')
													CHAR_AT(6)
														CHAR('a')
															CHAR_AT(7)
																CHAR('r')
																	CHAR_AT(8)
																		CHAR('e')
																			CHAR_AT(9)
																				CHAR('S')
																					CHAR_AT(10)
																						CHAR('u')
																							CHAR_AT(11)
																								CHAR('b')
																									CHAR_AT(12)
																										CHAR('s')
																											CHAR_AT(13)
																												CHAR('e')
																													CHAR_AT(14)
																														CHAR('t')
																															ON_LEAF_NOBR(15, "\xe2\x8a\x8f\xcc\xb8");  // &NotSquareSubset;
																															ON_SUFFIX(15, "Equal", "\xe2\x8b\xa2");  // &NotSquareSubsetEqual;
																													CHAR_END()
																											CHAR_END()
																									CHAR_END()
																								CHAR('p')
																									CHAR_AT(12)
																										CHAR('e')
																											CHAR_AT(13)
																												CHAR('r')
																													CHAR_AT(14)
																														CHAR('s')
																															CHAR_AT(15)
																																CHAR('e')
																																	CHAR_AT(16)
																																		CHAR('t')
																																			ON_LEAF_NOBR(17, "\xe2\x8a\x90\xcc\xb8");  // &NotSquareSuperset;
																																			ON_SUFFIX(17, "Equal", "\xe2\x8b\xa3");  // &NotSquareSupersetEqual;
																																	CHAR_END()
																															CHAR_END()
																													CHAR_END()
																											CHAR_END()
																									CHAR_END()
																							CHAR_END()
																					CHAR_END()
																			CHAR_END()
																	CHAR_END()
															CHAR_END()
													CHAR_END()
											CHAR_END()
										CHAR('u')
											CHAR_AT(5)
												CHAR('b')
													CHAR_AT(6)
														CHAR('s')
															CHAR_AT(7)
																CHAR('e')
																	CHAR_AT(8)
																		CHAR('t')
																			ON_LEAF_NOBR(9, "\xe2\x8a\x82\xe2\x83\x92");  // &NotSubset;
																			ON_SUFFIX(9, "Equal", "\xe2\x8a\x88");  // &NotSubsetEqual;
																	CHAR_END()
															CHAR_END()
													CHAR_END()
												CHAR('c')
													CHAR_AT(6)
														CHAR('c')
															CHAR_AT(7)
																CHAR('e')
																	CHAR_AT(8)
																		CHAR('e')
																			CHAR_AT(9)
																				CHAR('d')
																					CHAR_AT(10)
																						CHAR('s')
																							ON_LEAF_NOBR(11, "\xe2\x8a\x81");  // &NotSucceeds;
																							CHAR_AT(11)
																								CHAR('E') ON_SUFFIX(12, "qual", "\xe2\xaa\xb0\xcc\xb8");  // &NotSucceedsEqual;
																								CHAR('S') ON_SUFFIX(12, "lantEqual", "\xe2\x8b\xa1");  // &NotSucceedsSlantEqual;
																								CHAR('T') ON_SUFFIX(12, "ilde", "\xe2\x89\xbf\xcc\xb8");  // &NotSucceedsTilde;
																							CHAR_END()
																					CHAR_END()
																			CHAR_END()
																	CHAR_END()
															CHAR_END()
													CHAR_END()
												CHAR('p')
													CHAR_AT(6)
														CHAR('e')
															CHAR_AT(7)
																CHAR('r')
																	CHAR_AT(8)
																		CHAR('s')
																			CHAR_AT(9)
																				CHAR('e')
																					CHAR_AT(10)
																						CHAR('t')
																							ON_LEAF_NOBR(11, "\xe2\x8a\x83\xe2\x83\x92");  // &NotSuperset;
																							ON_SUFFIX(11, "Equal", "\xe2\x8a\x89");  // &NotSupersetEqual;
																					CHAR_END()
																			CHAR_END()
																	CHAR_END()
															CHAR_END()
													CHAR_END()
											CHAR_END()
									CHAR_END()
								CHAR('T')
									CHAR_AT(4)
										CHAR('i')
											CHAR_AT(5)
												CHAR('l')
													CHAR_AT(6)
														CHAR('d')
															CHAR_AT(7)
																CHAR('e')
																	ON_LEAF_NOBR(8, "\xe2\x89\x81");  // &NotTilde;
																	CHAR_AT(8)
																		CHAR('E') ON_SUFFIX(9, "qual", "\xe2\x89\x84");  // &NotTildeEqual;
																		CHAR('F') ON_SUFFIX(9, "ullEqual", "\xe2\x89\x87");  // &NotTildeFullEqual;
																		CHAR('T') ON_SUFFIX(9, "ilde", "\xe2\x89\x89");  // &NotTildeTilde;
																	CHAR_END()
															CHAR_END()
													CHAR_END()
											CHAR_END()
									CHAR_END()
								CHAR('V') ON_SUFFIX(4, "erticalBar", "\xe2\x88\xa4");  // &NotVerticalBar;
							CHAR_END()
					CHAR_END()
				CHAR('s') ON_SUFFIX(2, "cr", "\xf0\x9d\x92\xa9");  // &Nscr;
				CHAR('t') ON_SUFFIX(2, "ilde", "\xc3\x91");  // &Ntilde;
				CHAR('u') ON_LEAF(2, "\xce\x9d");  // &Nu;
			CHAR_END()
		CHAR('O')
			CHAR_AT(1)
				CHAR('E') ON_SUFFIX(2, "lig", "\xc5\x92");  // &OElig;
				CHAR('a') ON_SUFFIX(2, "cute", "\xc3\x93");  // &Oacute;
				CHAR('c')
					CHAR_AT(2)
						CHAR('i') ON_SUFFIX(3, "rc", "\xc3\x94");  // &Ocirc;
						CHAR('y') ON_LEAF(3, "\xd0\x9e");  // &Ocy;
					CHAR_END()
				CHAR('d') ON_SUFFIX(2, "blac", "\xc5\x90");  // &Odblac;
				CHAR('f') ON_SUFFIX(2, "r", "\xf0\x9d\x94\x92");  // &Ofr;
				CHAR('g') ON_SUFFIX(2, "rave", "\xc3\x92");  // &Ograve;
				CHAR('m')
					CHAR_AT(2)
						CHAR('a') ON_SUFFIX(3, "cr", "\xc5\x8c");  // &Omacr;
						CHAR('e') ON_SUFFIX(3, "ga", "\xce\xa9");  // &Omega;
						CHAR('i') ON_SUFFIX(3, "cron", "\xce\x9f");  // &Omicron;
					CHAR_END()
				CHAR('o') ON_SUFFIX(2, "pf", "\xf0\x9d\x95\x86");  // &Oopf;
				CHAR('p')
					CHAR_AT(2)
						CHAR('e')
							CHAR_AT(3)
								CHAR('n')
									CHAR_AT(4)
										CHAR('C')
											CHAR_AT(5)
												CHAR('u')
													CHAR_AT(6)
														CHAR('r')
															CHAR_AT(7)
																CHAR('l')
																	CHAR_AT(8)
																		CHAR('y')
																			CHAR_AT(9)
																				CHAR('D') ON_SUFFIX(10, "oubleQuote", "\xe2\x80\x9c");  // &OpenCurlyDoubleQuote;
																				CHAR('Q') ON_SUFFIX(10, "uote", "\xe2\x80\x98");  // &OpenCurlyQuote;
																			CHAR_END()
																	CHAR_END()
															CHAR_END()
													CHAR_END()
											CHAR_END()
									CHAR_END()
							CHAR_END()
					CHAR_END()
				CHAR('r') ON_LEAF(2, "\xe2\xa9\x94");  // &Or;
				CHAR('s')
					CHAR_AT(2)
						CHAR('c') ON_SUFFIX(3, "r", "\xf0\x9d\x92\xaa");  // &Oscr;
						CHAR('l') ON_SUFFIX(3, "ash", "\xc3\x98");  // &Oslash;
					CHAR_END()
				CHAR('t')
					CHAR_AT(2)
						CHAR('i')
							CHAR_AT(3)
								CHAR('l') ON_SUFFIX(4, "de", "\xc3\x95");  // &Otilde;
								CHAR('m') ON_SUFFIX(4, "es", "\xe2\xa8\xb7");  // &Otimes;
							CHAR_END()
					CHAR_END()
				CHAR('u') ON_SUFFIX(2, "ml", "\xc3\x96");  // &Ouml;
				CHAR('v')
					CHAR_AT(2)
						CHAR('e')
							CHAR_AT(3)
								CHAR('r')
									CHAR_AT(4)
										CHAR('B')
											CHAR_AT(5)
												CHAR('a') ON_SUFFIX(6, "r", "\xe2\x80\xbe");  // &OverBar;
												CHAR('r')
													CHAR_AT(6)
														CHAR('a')
															CHAR_AT(7)
																CHAR('c')
																	CHAR_AT(8)
																		CHAR('e') ON_LEAF(9, "\xe2\x8f\x9e");  // &OverBrace;
																		CHAR('k') ON_SUFFIX(9, "et", "\xe2\x8e\xb4");  // &OverBracket;
																	CHAR_END()
															CHAR_END()
													CHAR_END()
											CHAR_END()
										CHAR('P') ON_SUFFIX(5, "arenthesis", "\xe2\x8f\x9c");  // &OverParenthesis;
									CHAR_END()
							CHAR_END()
					CHAR_END()
			CHAR_END()
		CHAR('P')
			CHAR_AT(1)
				CHAR('a') ON_SUFFIX(2, "rtialD", "\xe2\x88\x82");  // &PartialD;
				CHAR('c') ON_SUFFIX(2, "y", "\xd0\x9f");  // &Pcy;
				CHAR('f') ON_SUFFIX(2, "r", "\xf0\x9d\x94\x93");  // &Pfr;
				CHAR('h') ON_SUFFIX(2, "i", "\xce\xa6");  // &Phi;
				CHAR('i') ON_LEAF(2, "\xce\xa0");  // &Pi;
				CHAR('l') ON_SUFFIX(2, "usMinus", "\xc2\xb1");  // &PlusMinus;
				CHAR('o')
					CHAR_AT(2)
						CHAR('i') ON_SUFFIX(3, "ncareplane", "\xe2\x84\x8c");  // &Poincareplane;
						CHAR('p') ON_SUFFIX(3, "f", "\xe2\x84\x99");  // &Popf;
					CHAR_END()
				CHAR('r')
					ON_LEAF_NOBR(2, "\xe2\xaa\xbb");  // &Pr;
					CHAR_AT(2)
						CHAR('e')
							CHAR_AT(3)
								CHAR('c')
									CHAR_AT(4)
										CHAR('e')
											CHAR_AT(5)
												CHAR('d')
													CHAR_AT(6)
														CHAR('e')
															CHAR_AT(7)
																CHAR('s')
																	ON_LEAF_NOBR(8, "\xe2\x89\xba");  // &Precedes;
																	CHAR_AT(8)
																		CHAR('E') ON_SUFFIX(9, "qual", "\xe2\xaa\xaf");  // &PrecedesEqual;
																		CHAR('S') ON_SUFFIX(9, "lantEqual", "\xe2\x89\xbc");  // &PrecedesSlantEqual;
																		CHAR('T') ON_SUFFIX(9, "ilde", "\xe2\x89\xbe");  // &PrecedesTilde;
																	CHAR_END()
															CHAR_END()
													CHAR_END()
											CHAR_END()
									CHAR_END()
							CHAR_END()
						CHAR('i') ON_SUFFIX(3, "me", "\xe2\x80\xb3");  // &Prime;
						CHAR('o')
							CHAR_AT(3)
								CHAR('d') ON_SUFFIX(4, "uct", "\xe2\x88\x8f");  // &Product;
								CHAR('p')
									CHAR_AT(4)
										CHAR('o')
											CHAR_AT(5)
												CHAR('r')
													CHAR_AT(6)
														CHAR('t')
															CHAR_AT(7)
																CHAR('i')
																	CHAR_AT(8)
																		CHAR('o')
																			CHAR_AT(9)
																				CHAR('n')
																					ON_LEAF_NOBR(10, "\xe2\x88\xb7");  // &Proportion;
																					ON_SUFFIX(10, "al", "\xe2\x88\x9d");  // &Proportional;
																			CHAR_END()
																	CHAR_END()
															CHAR_END()
													CHAR_END()
											CHAR_END()
									CHAR_END()
							CHAR_END()
					CHAR_END()
				CHAR('s')
					CHAR_AT(2)
						CHAR('c') ON_SUFFIX(3, "r", "\xf0\x9d\x92\xab");  // &Pscr;
						CHAR('i') ON_LEAF(3, "\xce\xa8");  // &Psi;
					CHAR_END()
			CHAR_END()
		CHAR('Q')
			CHAR_AT(1)
				CHAR('U') ON_SUFFIX(2, "OT", "\"");  // &QUOT;
				CHAR('f') ON_SUFFIX(2, "r", "\xf0\x9d\x94\x94");  // &Qfr;
				CHAR('o') ON_SUFFIX(2, "pf", "\xe2\x84\x9a");  // &Qopf;
				CHAR('s') ON_SUFFIX(2, "cr", "\xf0\x9d\x92\xac");  // &Qscr;
			CHAR_END()
		CHAR('R')
			CHAR_AT(1)
				CHAR('B') ON_SUFFIX(2, "arr", "\xe2\xa4\x90");  // &RBarr;
				CHAR('E') ON_SUFFIX(2, "G", "\xc2\xae");  // &REG;
				CHAR('a')
					CHAR_AT(2)
						CHAR('c') ON_SUFFIX(3, "ute", "\xc5\x94");  // &Racute;
						CHAR('n') ON_SUFFIX(3, "g", "\xe2\x9f\xab");  // &Rang;
						CHAR('r')
							CHAR_AT(3)
								CHAR('r')
									ON_LEAF_NOBR(4, "\xe2\x86\xa0");  // &Rarr;
									ON_SUFFIX(4, "tl", "\xe2\xa4\x96");  // &Rarrtl;
							CHAR_END()
					CHAR_END()
				CHAR('c')
					CHAR_AT(2)
						CHAR('a') ON_SUFFIX(3, "ron", "\xc5\x98");  // &Rcaron;
						CHAR('e') ON_SUFFIX(3, "dil", "\xc5\x96");  // &Rcedil;
						CHAR('y') ON_LEAF(3, "\xd0\xa0");  // &Rcy;
					CHAR_END()
				CHAR('e')
					ON_LEAF_NOBR(2, "\xe2\x84\x9c");  // &Re;
					CHAR_AT(2)
						CHAR('v')
							CHAR_AT(3)
								CHAR('e')
									CHAR_AT(4)
										CHAR('r')
											CHAR_AT(5)
												CHAR('s')
													CHAR_AT(6)
														CHAR('e')
															CHAR_AT(7)
																CHAR('E')
																	CHAR_AT(8)
																		CHAR('l') ON_SUFFIX(9, "ement", "\xe2\x88\x8b");  // &ReverseElement;
																		CHAR('q') ON_SUFFIX(9, "uilibrium", "\xe2\x87\x8b");  // &ReverseEquilibrium;
																	CHAR_END()
																CHAR('U') ON_SUFFIX(8, "pEquilibrium", "\xe2\xa5\xaf");  // &ReverseUpEquilibrium;
															CHAR_END()
													CHAR_END()
											CHAR_END()
									CHAR_END()
							CHAR_END()
					CHAR_END()
				CHAR('f') ON_SUFFIX(2, "r", "\xe2\x84\x9c");  // &Rfr;
				CHAR('h') ON_SUFFIX(2, "o", "\xce\xa1");  // &Rho;
				CHAR('i')
					CHAR_AT(2)
						CHAR('g')
							CHAR_AT(3)
								CHAR('h')
									CHAR_AT(4)
										CHAR('t')
											CHAR_AT(5)
												CHAR('A')
													CHAR_AT(6)
														CHAR('n') ON_SUFFIX(7, "gleBracket", "\xe2\x9f\xa9");  // &RightAngleBracket;
														CHAR('r')
															CHAR_AT(7)
																CHAR('r')
																	CHAR_AT(8)
																		CHAR('o')
																			CHAR_AT(9)
																				CHAR('w')
																					ON_LEAF_NOBR(10, "\xe2\x86\x92");  // &RightArrow;
																					CHAR_AT(10)
																						CHAR('B') ON_SUFFIX(11, "ar", "\xe2\x87\xa5");  // &RightArrowBar;
																						CHAR('L') ON_SUFFIX(11, "eftArrow", "\xe2\x87\x84");  // &RightArrowLeftArrow;
																					CHAR_END()
																			CHAR_END()
																	CHAR_END()
															CHAR_END()
													CHAR_END()
												CHAR('C') ON_SUFFIX(6, "eiling", "\xe2\x8c\x89");  // &RightCeiling;
												CHAR('D')
													CHAR_AT(6)
														CHAR('o')
															CHAR_AT(7)
																CHAR('u') ON_SUFFIX(8, "bleBracket", "\xe2\x9f\xa7");  // &RightDoubleBracket;
																CHAR('w')
																	CHAR_AT(8)
																		CHAR('n')
																			CHAR_AT(9)
																				CHAR('T') ON_SUFFIX(10, "eeVector", "\xe2\xa5\x9d");  // &RightDownTeeVector;
																				CHAR('V')
																					CHAR_AT(10)
																						CHAR('e')
																							CHAR_AT(11)
																								CHAR('c')
																									CHAR_AT(12)
																										CHAR('t')
																											CHAR_AT(13)
																												CHAR('o')
																													CHAR_AT(14)
																														CHAR('r')
																															ON_LEAF_NOBR(15, "\xe2\x87\x82");  // &RightDownVector;
																															ON_SUFFIX(15, "Bar", "\xe2\xa5\x95");  // &RightDownVectorBar;
																													CHAR_END()
																											CHAR_END()
																									CHAR_END()
																							CHAR_END()
																					CHAR_END()
																			CHAR_END()
																	CHAR_END()
															CHAR_END()
													CHAR_END()
												CHAR('F') ON_SUFFIX(6, "loor", "\xe2\x8c\x8b");  // &RightFloor;
												CHAR('T')
													CHAR_AT(6)
														CHAR('e')
															CHAR_AT(7)
																CHAR('e')
																	ON_LEAF_NOBR(8, "\xe2\x8a\xa2");  // &RightTee;
																	CHAR_AT(8)
																		CHAR('A') ON_SUFFIX(9, "rrow", "\xe2\x86\xa6");  // &RightTeeArrow;
																		CHAR('V') ON_SUFFIX(9, "ector", "\xe2\xa5\x9b");  // &RightTeeVector;
																	CHAR_END()
															CHAR_END()
														CHAR('r')
															CHAR_AT(7)
																CHAR('i')
																	CHAR_AT(8)
																		CHAR('a')
																			CHAR_AT(9)
																				CHAR('n')
																					CHAR_AT(10)
																						CHAR('g')
																							CHAR_AT(11)
																								CHAR('l')
																									CHAR_AT(12)
																										CHAR('e')
																											ON_LEAF_NOBR(13, "\xe2\x8a\xb3");  // &RightTriangle;
																											CHAR_AT(13)
																												CHAR('B') ON_SUFFIX(14, "ar", "\xe2\xa7\x90");  // &RightTriangleBar;
																												CHAR('E') ON_SUFFIX(14, "qual", "\xe2\x8a\xb5");  // &RightTriangleEqual;
																											CHAR_END()
																									CHAR_END()
																							CHAR_END()
																					CHAR_END()
																			CHAR_END()
																	CHAR_END()
															CHAR_END()
													CHAR_END()
												CHAR('U')
													CHAR_AT(6)
														CHAR('p')
															CHAR_AT(7)
																CHAR('D') ON_SUFFIX(8, "ownVector", "\xe2\xa5\x8f");  // &RightUpDownVector;
																CHAR('T') ON_SUFFIX(8, "eeVector", "\xe2\xa5\x9c");  // &RightUpTeeVector;
																CHAR('V')
																	CHAR_AT(8)
																		CHAR('e')
																			CHAR_AT(9)
																				CHAR('c')
																					CHAR_AT(10)
																						CHAR('t')
																							CHAR_AT(11)
																								CHAR('o')
																									CHAR_AT(12)
																										CHAR('r')
																											ON_LEAF_NOBR(13, "\xe2\x86\xbe");  // &RightUpVector;
																											ON_SUFFIX(13, "Bar", "\xe2\xa5\x94");  // &RightUpVectorBar;
																									CHAR_END()
																							CHAR_END()
																					CHAR_END()
																			CHAR_END()
																	CHAR_END()
															CHAR_END()
													CHAR_END()
												CHAR('V')
													CHAR_AT(6)
														CHAR('e')
															CHAR_AT(7)
																CHAR('c')
																	CHAR_AT(8)
																		CHAR('t')
																			CHAR_AT(9)
																				CHAR('o')
																					CHAR_AT(10)
																						CHAR('r')
																							ON_LEAF_NOBR(11, "\xe2\x87\x80");  // &RightVector;
																							ON_SUFFIX(11, "Bar", "\xe2\xa5\x93");  // &RightVectorBar;
																					CHAR_END()
																			CHAR_END()
																	CHAR_END()
															CHAR_END()
													CHAR_END()
												CHAR('a') ON_SUFFIX(6, "rrow", "\xe2\x87\x92");  // &Rightarrow;
											CHAR_END()
									CHAR_END()
							CHAR_END()
					CHAR_END()
				CHAR('o')
					CHAR_AT(2)
						CHAR('p') ON_SUFFIX(3, "f", "\xe2\x84\x9d");  // &Ropf;
						CHAR('u') ON_SUFFIX(3, "ndImplies", "\xe2\xa5\xb0");  // &RoundImplies;
					CHAR_END()
				CHAR('r') ON_SUFFIX(2, "ightarrow", "\xe2\x87\x9b");  // &Rrightarrow;
				CHAR('s')
					CHAR_AT(2)
						CHAR('c') ON_SUFFIX(3, "r", "\xe2\x84\x9b");  // &Rscr;
						CHAR('h') ON_LEAF(3, "\xe2\x86\xb1");  // &Rsh;
					CHAR_END()
				CHAR('u') ON_SUFFIX(2, "leDelayed", "\xe2\xa7\xb4");  // &RuleDelayed;
			CHAR_END()
		CHAR('S')
			CHAR_AT(1)
				CHAR('H')
					CHAR_AT(2)
						CHAR('C') ON_SUFFIX(3, "Hcy", "\xd0\xa9");  // &SHCHcy;
						CHAR('c') ON_SUFFIX(3, "y", "\xd0\xa8");  // &SHcy;
					CHAR_END()
				CHAR('O') ON_SUFFIX(2, "FTcy", "\xd0\xac");  // &SOFTcy;
				CHAR('a') ON_SUFFIX(2, "cute", "\xc5\x9a");  // &Sacute;
				CHAR('c')
					ON_LEAF_NOBR(2, "\xe2\xaa\xbc");  // &Sc;
					CHAR_AT(2)
						CHAR('a') ON_SUFFIX(3, "ron", "\xc5\xa0");  // &Scaron;
						CHAR('e') ON_SUFFIX(3, "dil", "\xc5\x9e");  // &Scedil;
						CHAR('i') ON_SUFFIX(3, "rc", "\xc5\x9c");  // &Scirc;
						CHAR('y') ON_LEAF(3, "\xd0\xa1");  // &Scy;
					CHAR_END()
				CHAR('f') ON_SUFFIX(2, "r", "\xf0\x9d\x94\x96");  // &Sfr;
				CHAR('h')
					CHAR_AT(2)
						CHAR('o')
							CHAR_AT(3)
								CHAR('r')
									CHAR_AT(4)
										CHAR('t')
											CHAR_AT(5)
												CHAR('D') ON_SUFFIX(6, "ownArrow", "\xe2\x86\x93");  // &ShortDownArrow;
												CHAR('L') ON_SUFFIX(6, "eftArrow", "\xe2\x86\x90");  // &ShortLeftArrow;
												CHAR('R') ON_SUFFIX(6, "ightArrow", "\xe2\x86\x92");  // &ShortRightArrow;
												CHAR('U') ON_SUFFIX(6, "pArrow", "\xe2\x86\x91");  // &ShortUpArrow;
											CHAR_END()
									CHAR_END()
							CHAR_END()
					CHAR_END()
				CHAR('i') ON_SUFFIX(2, "gma", "\xce\xa3");  // &Sigma;
				CHAR('m') ON_SUFFIX(2, "allCircle", "\xe2\x88\x98");  // &SmallCircle;
				CHAR('o') ON_SUFFIX(2, "pf", "\xf0\x9d\x95\x8a");  // &Sopf;
				CHAR('q')
					CHAR_AT(2)
						CHAR('r') ON_SUFFIX(3, "t", "\xe2\x88\x9a");  // &Sqrt;
						CHAR('u')
							CHAR_AT(3)
								CHAR('a')
									CHAR_AT(4)
										CHAR('r')
											CHAR_AT(5)
												CHAR('e')
													ON_LEAF_NOBR(6, "\xe2\x96\xa1");  // &Square;
													CHAR_AT(6)
														CHAR('I') ON_SUFFIX(7, "ntersection", "\xe2\x8a\x93");  // &SquareIntersection;
														CHAR('S')
															CHAR_AT(7)
																CHAR('u')
																	CHAR_AT(8)
																		CHAR('b')
																			CHAR_AT(9)
																				CHAR('s')
																					CHAR_AT(10)
																						CHAR('e')
																							CHAR_AT(11)
																								CHAR('t')
																									ON_LEAF_NOBR(12, "\xe2\x8a\x8f");  // &SquareSubset;
																									ON_SUFFIX(12, "Equal", "\xe2\x8a\x91");  // &SquareSubsetEqual;
																							CHAR_END()
																					CHAR_END()
																			CHAR_END()
																		CHAR('p')
																			CHAR_AT(9)
																				CHAR('e')
																					CHAR_AT(10)
																						CHAR('r')
																							CHAR_AT(11)
																								CHAR('s')
																									CHAR_AT(12)
																										CHAR('e')
																											CHAR_AT(13)
																												CHAR('t')
																													ON_LEAF_NOBR(14, "\xe2\x8a\x90");  // &SquareSuperset;
																													ON_SUFFIX(14, "Equal", "\xe2\x8a\x92");  // &SquareSupersetEqual;
																											CHAR_END()
																									CHAR_END()
																							CHAR_END()
																					CHAR_END()
																			CHAR_END()
																	CHAR_END()
															CHAR_END()
														CHAR('U') ON_SUFFIX(7, "nion", "\xe2\x8a\x94");  // &SquareUnion;
													CHAR_END()
											CHAR_END()
									CHAR_END()
							CHAR_END()
					CHAR_END()
				CHAR('s') ON_SUFFIX(2, "cr", "\xf0\x9d\x92\xae");  // &Sscr;
				CHAR('t') ON_SUFFIX(2, "ar", "\xe2\x8b\x86");  // &Star;
				CHAR('u')
					CHAR_AT(2)
						CHAR('b')
							ON_LEAF_NOBR(3, "\xe2\x8b\x90");  // &Sub;
							CHAR_AT(3)
								CHAR('s')
									CHAR_AT(4)
										CHAR('e')
											CHAR_AT(5)
												CHAR('t')
													ON_LEAF_NOBR(6, "\xe2\x8b\x90");  // &Subset;
													ON_SUFFIX(6, "Equal", "\xe2\x8a\x86");  // &SubsetEqual;
											CHAR_END()
									CHAR_END()
							CHAR_END()
						CHAR('c')
							CHAR_AT(3)
								CHAR('c')
									CHAR_AT(4)
										CHAR('e')
											CHAR_AT(5)
												CHAR('e')
													CHAR_AT(6)
														CHAR('d')
															CHAR_AT(7)
																CHAR('s')
																	ON_LEAF_NOBR(8, "\xe2\x89\xbb");  // &Succeeds;
																	CHAR_AT(8)
																		CHAR('E') ON_SUFFIX(9, "qual", "\xe2\xaa\xb0");  // &SucceedsEqual;
																		CHAR('S') ON_SUFFIX(9, "lantEqual", "\xe2\x89\xbd");  // &SucceedsSlantEqual;
																		CHAR('T') ON_SUFFIX(9, "ilde", "\xe2\x89\xbf");  // &SucceedsTilde;
																	CHAR_END()
															CHAR_END()
													CHAR_END()
											CHAR_END()
									CHAR_END()
								CHAR('h') ON_SUFFIX(4, "That", "\xe2\x88\x8b");  // &SuchThat;
							CHAR_END()
						CHAR('m') ON_LEAF(3, "\xe2\x88\x91");  // &Sum;
						CHAR('p')
							ON_LEAF_NOBR(3, "\xe2\x8b\x91");  // &Sup;
							CHAR_AT(3)
								CHAR('e')
									CHAR_AT(4)
										CHAR('r')
											CHAR_AT(5)
												CHAR('s')
													CHAR_AT(6)
														CHAR('e')
															CHAR_AT(7)
																CHAR('t')
																	ON_LEAF_NOBR(8, "\xe2\x8a\x83");  // &Superset;
																	ON_SUFFIX(8, "Equal", "\xe2\x8a\x87");  // &SupersetEqual;
															CHAR_END()
													CHAR_END()
											CHAR_END()
									CHAR_END()
								CHAR('s') ON_SUFFIX(4, "et", "\xe2\x8b\x91");  // &Supset;
							CHAR_END()
					CHAR_END()
			CHAR_END()
		CHAR('T')
			CHAR_AT(1)
				CHAR('H') ON_SUFFIX(2, "ORN", "\xc3\x9e");  // &THORN;
				CHAR('R') ON_SUFFIX(2, "ADE", "\xe2\x84\xa2");  // &TRADE;
				CHAR('S')
					CHAR_AT(2)
						CHAR('H') ON_SUFFIX(3, "cy", "\xd0\x8b");  // &TSHcy;
						CHAR('c') ON_SUFFIX(3, "y", "\xd0\xa6");  // &TScy;
					CHAR_END()
				CHAR('a')
					CHAR_AT(2)
						CHAR('b') ON_LEAF(3, "\x09");  // &Tab;
						CHAR('u') ON_LEAF(3, "\xce\xa4");  // &Tau;
					CHAR_END()
				CHAR('c')
					CHAR_AT(2)
						CHAR('a') ON_SUFFIX(3, "ron", "\xc5\xa4");  // &Tcaron;
						CHAR('e') ON_SUFFIX(3, "dil", "\xc5\xa2");  // &Tcedil;
						CHAR('y') ON_LEAF(3, "\xd0\xa2");  // &Tcy;
					CHAR_END()
				CHAR('f') ON_SUFFIX(2, "r", "\xf0\x9d\x94\x97");  // &Tfr;
				CHAR('h')
					CHAR_AT(2)
						CHAR('e')
							CHAR_AT(3)
								CHAR('r') ON_SUFFIX(4, "efore", "\xe2\x88\xb4");  // &Therefore;
								CHAR('t') ON_SUFFIX(4, "a", "\xce\x98");  // &Theta;
							CHAR_END()
						CHAR('i')
							CHAR_AT(3)
								CHAR('c') ON_SUFFIX(4, "kSpace", "\xe2\x81\x9f\xe2\x80\x8a");  // &ThickSpace;
								CHAR('n') ON_SUFFIX(4, "Space", "\xe2\x80\x89");  // &ThinSpace;
							CHAR_END()
					CHAR_END()
				CHAR('i')
					CHAR_AT(2)
						CHAR('l')
							CHAR_AT(3)
								CHAR('d')
									CHAR_AT(4)
										CHAR('e')
											ON_LEAF_NOBR(5, "\xe2\x88\xbc");  // &Tilde;
											CHAR_AT(5)
												CHAR('E') ON_SUFFIX(6, "qual", "\xe2\x89\x83");  // &TildeEqual;
												CHAR('F') ON_SUFFIX(6, "ullEqual", "\xe2\x89\x85");  // &TildeFullEqual;
												CHAR('T') ON_SUFFIX(6, "ilde", "\xe2\x89\x88");  // &TildeTilde;
											CHAR_END()
									CHAR_END()
							CHAR_END()
					CHAR_END()
				CHAR('o') ON_SUFFIX(2, "pf", "\xf0\x9d\x95\x8b");  // &Topf;
				CHAR('r') ON_SUFFIX(2, "ipleDot", "\xe2\x83\x9b");  // &TripleDot;
				CHAR('s')
					CHAR_AT(2)
						CHAR('c') ON_SUFFIX(3, "r", "\xf0\x9d\x92\xaf");  // &Tscr;
						CHAR('t') ON_SUFFIX(3, "rok", "\xc5\xa6");  // &Tstrok;
					CHAR_END()
			CHAR_END()
		CHAR('U')
			CHAR_AT(1)
				CHAR('a')
					CHAR_AT(2)
						CHAR('c') ON_SUFFIX(3, "ute", "\xc3\x9a");  // &Uacute;
						CHAR('r')
							CHAR_AT(3)
								CHAR('r')
									ON_LEAF_NOBR(4, "\xe2\x86\x9f");  // &Uarr;
									ON_SUFFIX(4, "ocir", "\xe2\xa5\x89");  // &Uarrocir;
							CHAR_END()
					CHAR_END()
				CHAR('b')
					CHAR_AT(2)
						CHAR('r')
							CHAR_AT(3)
								CHAR('c') ON_SUFFIX(4, "y", "\xd0\x8e");  // &Ubrcy;
								CHAR('e') ON_SUFFIX(4, "ve", "\xc5\xac");  // &Ubreve;
							CHAR_END()
					CHAR_END()
				CHAR('c')
					CHAR_AT(2)
						CHAR('i') ON_SUFFIX(3, "rc", "\xc3\x9b");  // &Ucirc;
						CHAR('y') ON_LEAF(3, "\xd0\xa3");  // &Ucy;
					CHAR_END()
				CHAR('d') ON_SUFFIX(2, "blac", "\xc5\xb0");  // &Udblac;
				CHAR('f') ON_SUFFIX(2, "r", "\xf0\x9d\x94\x98");  // &Ufr;
				CHAR('g') ON_SUFFIX(2, "rave", "\xc3\x99");  // &Ugrave;
				CHAR('m') ON_SUFFIX(2, "acr", "\xc5\xaa");  // &Umacr;
				CHAR('n')
					CHAR_AT(2)
						CHAR('d')
							CHAR_AT(3)
								CHAR('e')
									CHAR_AT(4)
										CHAR('r')
											CHAR_AT(5)
												CHAR('B')
													CHAR_AT(6)
														CHAR('a') ON_SUFFIX(7, "r", "_");  // &UnderBar;
														CHAR('r')
															CHAR_AT(7)
																CHAR('a')
																	CHAR_AT(8)
																		CHAR('c')
																			CHAR_AT(9)
																				CHAR('e') ON_LEAF(10, "\xe2\x8f\x9f");  // &UnderBrace;
																				CHAR('k') ON_SUFFIX(10, "et", "\xe2\x8e\xb5");  // &UnderBracket;
																			CHAR_END()
																	CHAR_END()
															CHAR_END()
													CHAR_END()
												CHAR('P') ON_SUFFIX(6, "arenthesis", "\xe2\x8f\x9d");  // &UnderParenthesis;
											CHAR_END()
									CHAR_END()
							CHAR_END()
						CHAR('i')
							CHAR_AT(3)
								CHAR('o')
									CHAR_AT(4)
										CHAR('n')
											ON_LEAF_NOBR(5, "\xe2\x8b\x83");  // &Union;
											ON_SUFFIX(5, "Plus", "\xe2\x8a\x8e");  // &UnionPlus;
									CHAR_END()
							CHAR_END()
					CHAR_END()
				CHAR('o')
					CHAR_AT(2)
						CHAR('g') ON_SUFFIX(3, "on", "\xc5\xb2");  // &Uogon;
						CHAR('p') ON_SUFFIX(3, "f", "\xf0\x9d\x95\x8c");  // &Uopf;
					CHAR_END()
				CHAR('p')
					CHAR_AT(2)
						CHAR('A')
							CHAR_AT(3)
								CHAR('r')
									CHAR_AT(4)
										CHAR('r')
											CHAR_AT(5)
												CHAR('o')
													CHAR_AT(6)
														CHAR('w')
															ON_LEAF_NOBR(7, "\xe2\x86\x91");  // &UpArrow;
															CHAR_AT(7)
																CHAR('B') ON_SUFFIX(8, "ar", "\xe2\xa4\x92");  // &UpArrowBar;
																CHAR('D') ON_SUFFIX(8, "ownArrow", "\xe2\x87\x85");  // &UpArrowDownArrow;
															CHAR_END()
													CHAR_END()
											CHAR_END()
									CHAR_END()
							CHAR_END()
						CHAR('D') ON_SUFFIX(3, "ownArrow", "\xe2\x86\x95");  // &UpDownArrow;
						CHAR('E') ON_SUFFIX(3, "quilibrium", "\xe2\xa5\xae");  // &UpEquilibrium;
						CHAR('T')
							CHAR_AT(3)
								CHAR('e')
									CHAR_AT(4)
										CHAR('e')
											ON_LEAF_NOBR(5, "\xe2\x8a\xa5");  // &UpTee;
											ON_SUFFIX(5, "Arrow", "\xe2\x86\xa5");  // &UpTeeArrow;
									CHAR_END()
							CHAR_END()
						CHAR('a') ON_SUFFIX(3, "rrow", "\xe2\x87\x91");  // &Uparrow;
						CHAR('d') ON_SUFFIX(3, "ownarrow", "\xe2\x87\x95");  // &Updownarrow;
						CHAR('p')
							CHAR_AT(3)
								CHAR('e')
									CHAR_AT(4)
										CHAR('r')
											CHAR_AT(5)
												CHAR('L') ON_SUFFIX(6, "eftArrow", "\xe2\x86\x96");  // &UpperLeftArrow;
												CHAR('R') ON_SUFFIX(6, "ightArrow", "\xe2\x86\x97");  // &UpperRightArrow;
											CHAR_END()
									CHAR_END()
							CHAR_END()
						CHAR('s')
							CHAR_AT(3)
								CHAR('i')
									ON_LEAF_NOBR(4, "\xcf\x92");  // &Upsi;
									ON_SUFFIX(4, "lon", "\xce\xa5");  // &Upsilon;
							CHAR_END()
					CHAR_END()
				CHAR('r') ON_SUFFIX(2, "ing", "\xc5\xae");  // &Uring;
				CHAR('s') ON_SUFFIX(2, "cr", "\xf0\x9d\x92\xb0");  // &Uscr;
				CHAR('t') ON_SUFFIX(2, "ilde", "\xc5\xa8");  // &Utilde;
				CHAR('u') ON_SUFFIX(2, "ml", "\xc3\x9c");  // &Uuml;
			CHAR_END()
		CHAR('V')
			CHAR_AT(1)
				CHAR('D') ON_SUFFIX(2, "ash", "\xe2\x8a\xab");  // &VDash;
				CHAR('b') ON_SUFFIX(2, "ar", "\xe2\xab\xab");  // &Vbar;
				CHAR('c') ON_SUFFIX(2, "y", "\xd0\x92");  // &Vcy;
				CHAR('d')
					CHAR_AT(2)
						CHAR('a')
							CHAR_AT(3)
								CHAR('s')
									CHAR_AT(4)
										CHAR('h')
											ON_LEAF_NOBR(5, "\xe2\x8a\xa9");  // &Vdash;
											ON_SUFFIX(5, "l", "\xe2\xab\xa6");  // &Vdashl;
									CHAR_END()
							CHAR_END()
					CHAR_END()
				CHAR('e')
					CHAR_AT(2)
						CHAR('e') ON_LEAF(3, "\xe2\x8b\x81");  // &Vee;
						CHAR('r')
							CHAR_AT(3)
								CHAR('b') ON_SUFFIX(4, "ar", "\xe2\x80\x96");  // &Verbar;
								CHAR('t')
									ON_LEAF_NOBR(4, "\xe2\x80\x96");  // &Vert;
									CHAR_AT(4)
										CHAR('i')
											CHAR_AT(5)
												CHAR('c')
													CHAR_AT(6)
														CHAR('a')
															CHAR_AT(7)
																CHAR('l')
																	CHAR_AT(8)
																		CHAR('B') ON_SUFFIX(9, "ar", "\xe2\x88\xa3");  // &VerticalBar;
																		CHAR('L') ON_SUFFIX(9, "ine", "|");  // &VerticalLine;
																		CHAR('S') ON_SUFFIX(9, "eparator", "\xe2\x9d\x98");  // &VerticalSeparator;
																		CHAR('T') ON_SUFFIX(9, "ilde", "\xe2\x89\x80");  // &VerticalTilde;
																	CHAR_END()
															CHAR_END()
													CHAR_END()
											CHAR_END()
									CHAR_END()
								CHAR('y') ON_SUFFIX(4, "ThinSpace", "\xe2\x80\x8a");  // &VeryThinSpace;
							CHAR_END()
					CHAR_END()
				CHAR('f') ON_SUFFIX(2, "r", "\xf0\x9d\x94\x99");  // &Vfr;
				CHAR('o') ON_SUFFIX(2, "pf", "\xf0\x9d\x95\x8d");  // &Vopf;
				CHAR('s') ON_SUFFIX(2, "cr", "\xf0\x9d\x92\xb1");  // &Vscr;
				CHAR('v') ON_SUFFIX(2, "dash", "\xe2\x8a\xaa");  // &Vvdash;
			CHAR_END()
		CHAR('W')
			CHAR_AT(1)
				CHAR('c') ON_SUFFIX(2, "irc", "\xc5\xb4");  // &Wcirc;
				CHAR('e') ON_SUFFIX(2, "dge", "\xe2\x8b\x80");  // &Wedge;
				CHAR('f') ON_SUFFIX(2, "r", "\xf0\x9d\x94\x9a");  // &Wfr;
				CHAR('o') ON_SUFFIX(2, "pf", "\xf0\x9d\x95\x8e");  // &Wopf;
				CHAR('s') ON_SUFFIX(2, "cr", "\xf0\x9d\x92\xb2");  // &Wscr;
			CHAR_END()
		CHAR('X')
			CHAR_AT(1)
				CHAR('f') ON_SUFFIX(2, "r", "\xf0\x9d\x94\x9b");  // &Xfr;
				CHAR('i') ON_LEAF(2, "\xce\x9e");  // &Xi;
				CHAR('o') ON_SUFFIX(2, "pf", "\xf0\x9d\x95\x8f");  // &Xopf;
				CHAR('s') ON_SUFFIX(2, "cr", "\xf0\x9d\x92\xb3");  // &Xscr;
			CHAR_END()
		CHAR('Y')
			CHAR_AT(1)
				CHAR('A') ON_SUFFIX(2, "cy", "\xd0\xaf");  // &YAcy;
				CHAR('I') ON_SUFFIX(2, "cy", "\xd0\x87");  // &YIcy;
				CHAR('U') ON_SUFFIX(2, "cy", "\xd0\xae");  // &YUcy;
				CHAR('a') ON_SUFFIX(2, "cute", "\xc3\x9d");  // &Yacute;
				CHAR('c')
					CHAR_AT(2)
						CHAR('i') ON_SUFFIX(3, "rc", "\xc5\xb6");  // &Ycirc;
						CHAR('y') ON_LEAF(3, "\xd0\xab");  // &Ycy;
					CHAR_END()
				CHAR('f') ON_SUFFIX(2, "r", "\xf0\x9d\x94\x9c");  // &Yfr;
				CHAR('o') ON_SUFFIX(2, "pf", "\xf0\x9d\x95\x90");  // &Yopf;
				CHAR('s') ON_SUFFIX(2, "cr", "\xf0\x9d\x92\xb4");  // &Yscr;
				CHAR('u') ON_SUFFIX(2, "ml", "\xc5\xb8");  // &Yuml;
			CHAR_END()
		CHAR('Z')
			CHAR_AT(1)
				CHAR('H') ON_SUFFIX(2, "cy", "\xd0\x96");  // &ZHcy;
				CHAR('a') ON_SUFFIX(2, "cute", "\xc5\xb9");  // &Zacute;
				CHAR('c')
					CHAR_AT(2)
						CHAR('a') ON_SUFFIX(3, "ron", "\xc5\xbd");  // &Zcaron;
						CHAR('y') ON_LEAF(3, "\xd0\x97");  // &Zcy;
					CHAR_END()
				CHAR('d') ON_SUFFIX(2, "ot", "\xc5\xbb");  // &Zdot;
				CHAR('e')
					CHAR_AT(2)
						CHAR('r') ON_SUFFIX(3, "oWidthSpace", "\xe2\x80\x8b");  // &ZeroWidthSpace;
						CHAR('t') ON_SUFFIX(3, "a", "\xce\x96");  // &Zeta;
					CHAR_END()
				CHAR('f') ON_SUFFIX(2, "r", "\xe2\x84\xa8");  // &Zfr;
				CHAR('o') ON_SUFFIX(2, "pf", "\xe2\x84\xa4");  // &Zopf;
				CHAR('s') ON_SUFFIX(2, "cr", "\xf0\x9d\x92\xb5");  // &Zscr;
			CHAR_END()
		CHAR('a')
			CHAR_AT(1)
				CHAR('a') ON_SUFFIX(2, "cute", "\xc3\xa1");  // &aacute;
				CHAR('b') ON_SUFFIX(2, "reve", "\xc4\x83");  // &abreve;
				CHAR('c')
					ON_LEAF_NOBR(2, "\xe2\x88\xbe");  // &ac;
					CHAR_AT(2)
						CHAR('E') ON_LEAF(3, "\xe2\x88\xbe\xcc\xb3");  // &acE;
						CHAR('d') ON_LEAF(3, "\xe2\x88\xbf");  // &acd;
						CHAR('i') ON_SUFFIX(3, "rc", "\xc3\xa2");  // &acirc;
						CHAR('u') ON_SUFFIX(3, "te", "\xc2\xb4");  // &acute;
						CHAR('y') ON_LEAF(3, "\xd0\xb0");  // &acy;
					CHAR_END()
				CHAR('e') ON_SUFFIX(2, "lig", "\xc3\xa6");  // &aelig;
				CHAR('f')
					ON_LEAF_NOBR(2, "\xe2\x81\xa1");  // &af;
					ON_SUFFIX(2, "r", "\xf0\x9d\x94\x9e");  // &afr;
				CHAR('g') ON_SUFFIX(2, "rave", "\xc3\xa0");  // &agrave;
				CHAR('l')
					CHAR_AT(2)
						CHAR('e')
							CHAR_AT(3)
								CHAR('f') ON_SUFFIX(4, "sym", "\xe2\x84\xb5");  // &alefsym;
								CHAR('p') ON_SUFFIX(4, "h", "\xe2\x84\xb5");  // &aleph;
							CHAR_END()
						CHAR('p') ON_SUFFIX(3, "ha", "\xce\xb1");  // &alpha;
					CHAR_END()
				CHAR('m')
					CHAR_AT(2)
						CHAR('a')
							CHAR_AT(3)
								CHAR('c') ON_SUFFIX(4, "r", "\xc4\x81");  // &amacr;
								CHAR('l') ON_SUFFIX(4, "g", "\xe2\xa8\xbf");  // &amalg;
							CHAR_END()
						CHAR('p') ON_LEAF(3, "&");  // &amp;
					CHAR_END()
				CHAR('n')
					CHAR_AT(2)
						CHAR('d')
							ON_LEAF_NOBR(3, "\xe2\x88\xa7");  // &and;
							CHAR_AT(3)
								CHAR('a') ON_SUFFIX(4, "nd", "\xe2\xa9\x95");  // &andand;
								CHAR('d') ON_LEAF(4, "\xe2\xa9\x9c");  // &andd;
								CHAR('s') ON_SUFFIX(4, "lope", "\xe2\xa9\x98");  // &andslope;
								CHAR('v') ON_LEAF(4, "\xe2\xa9\x9a");  // &andv;
							CHAR_END()
						CHAR('g')
							ON_LEAF_NOBR(3, "\xe2\x88\xa0");  // &ang;
							CHAR_AT(3)
								CHAR('e') ON_LEAF(4, "\xe2\xa6\xa4");  // &ange;
								CHAR('l') ON_SUFFIX(4, "e", "\xe2\x88\xa0");  // &angle;
								CHAR('m')
									CHAR_AT(4)
										CHAR('s')
											CHAR_AT(5)
												CHAR('d')
													ON_LEAF_NOBR(6, "\xe2\x88\xa1");  // &angmsd;
													CHAR_AT(6)
														CHAR('a')
															CHAR_AT(7)
																CHAR('a') ON_LEAF(8, "\xe2\xa6\xa8");  // &angmsdaa;
																CHAR('b') ON_LEAF(8, "\xe2\xa6\xa9");  // &angmsdab;
																CHAR('c') ON_LEAF(8, "\xe2\xa6\xaa");  // &angmsdac;
																CHAR('d') ON_LEAF(8, "\xe2\xa6\xab");  // &angmsdad;
																CHAR('e') ON_LEAF(8, "\xe2\xa6\xac");  // &angmsdae;
																CHAR('f') ON_LEAF(8, "\xe2\xa6\xad");  // &angmsdaf;
																CHAR('g') ON_LEAF(8, "\xe2\xa6\xae");  // &angmsdag;
																CHAR('h') ON_LEAF(8, "\xe2\xa6\xaf");  // &angmsdah;
															CHAR_END()
													CHAR_END()
											CHAR_END()
									CHAR_END()
								CHAR('r')
									CHAR_AT(4)
										CHAR('t')
											ON_LEAF_NOBR(5, "\xe2\x88\x9f");  // &angrt;
											CHAR_AT(5)
												CHAR('v')
													CHAR_AT(6)
														CHAR('b')
															ON_LEAF_NOBR(7, "\xe2\x8a\xbe");  // &angrtvb;
															ON_SUFFIX(7, "d", "\xe2\xa6\x9d");  // &angrtvbd;
													CHAR_END()
											CHAR_END()
									CHAR_END()
								CHAR('s')
									CHAR_AT(4)
										CHAR('p') ON_SUFFIX(5, "h", "\xe2\x88\xa2");  // &angsph;
										CHAR('t') ON_LEAF(5, "\xc3\x85");  // &angst;
									CHAR_END()
								CHAR('z') ON_SUFFIX(4, "arr", "\xe2\x8d\xbc");  // &angzarr;
							CHAR_END()
					CHAR_END()
				CHAR('o')
					CHAR_AT(2)
						CHAR('g') ON_SUFFIX(3, "on", "\xc4\x85");  // &aogon;
						CHAR('p') ON_SUFFIX(3, "f", "\xf0\x9d\x95\x92");  // &aopf;
					CHAR_END()
				CHAR('p')
					ON_LEAF_NOBR(2, "\xe2\x89\x88");  // &ap;
					CHAR_AT(2)
						CHAR('E') ON_LEAF(3, "\xe2\xa9\xb0");  // &apE;
						CHAR('a') ON_SUFFIX(3, "cir", "\xe2\xa9\xaf");  // &apacir;
						CHAR('e') ON_LEAF(3, "\xe2\x89\x8a");  // &ape;
						CHAR('i') ON_SUFFIX(3, "d", "\xe2\x89\x8b");  // &apid;
						CHAR('o') ON_SUFFIX(3, "s", "'");  // &apos;
						CHAR('p')
							CHAR_AT(3)
								CHAR('r')
									CHAR_AT(4)
										CHAR('o')
											CHAR_AT(5)
												CHAR('x')
													ON_LEAF_NOBR(6, "\xe2\x89\x88");  // &approx;
													ON_SUFFIX(6, "eq", "\xe2\x89\x8a");  // &approxeq;
											CHAR_END()
									CHAR_END()
							CHAR_END()
					CHAR_END()
				CHAR('r') ON_SUFFIX(2, "ing", "\xc3\xa5");  // &aring;
				CHAR('s')
					CHAR_AT(2)
						CHAR('c') ON_SUFFIX(3, "r", "\xf0\x9d\x92\xb6");  // &ascr;
						CHAR('t') ON_LEAF(3, "*");  // &ast;
						CHAR('y')
							CHAR_AT(3)
								CHAR('m')
									CHAR_AT(4)
										CHAR('p')
											ON_LEAF_NOBR(5, "\xe2\x89\x88");  // &asymp;
											ON_SUFFIX(5, "eq", "\xe2\x89\x8d");  // &asympeq;
									CHAR_END()
							CHAR_END()
					CHAR_END()
				CHAR('t') ON_SUFFIX(2, "ilde", "\xc3\xa3");  // &atilde;
				CHAR('u') ON_SUFFIX(2, "ml", "\xc3\xa4");  // &auml;
				CHAR('w')
					CHAR_AT(2)
						CHAR('c') ON_SUFFIX(3, "onint", "\xe2\x88\xb3");  // &awconint;
						CHAR('i') ON_SUFFIX(3, "nt", "\xe2\xa8\x91");  // &awint;
					CHAR_END()
			CHAR_END()
		CHAR('b')
			CHAR_AT(1)
				CHAR('N') ON_SUFFIX(2, "ot", "\xe2\xab\xad");  // &bNot;
				CHAR('a')
					CHAR_AT(2)
						CHAR('c')
							CHAR_AT(3)
								CHAR('k')
									CHAR_AT(4)
										CHAR('c') ON_SUFFIX(5, "ong", "\xe2\x89\x8c");  // &backcong;
										CHAR('e') ON_SUFFIX(5, "psilon", "\xcf\xb6");  // &backepsilon;
										CHAR('p') ON_SUFFIX(5, "rime", "\xe2\x80\xb5");  // &backprime;
										CHAR('s')
											CHAR_AT(5)
												CHAR('i')
													CHAR_AT(6)
														CHAR('m')
															ON_LEAF_NOBR(7, "\xe2\x88\xbd");  // &backsim;
															ON_SUFFIX(7, "eq", "\xe2\x8b\x8d");  // &backsimeq;
													CHAR_END()
											CHAR_END()
									CHAR_END()
							CHAR_END()
						CHAR('r')
							CHAR_AT(3)
								CHAR('v') ON_SUFFIX(4, "ee", "\xe2\x8a\xbd");  // &barvee;
								CHAR('w')
									CHAR_AT(4)
										CHAR('e')
											CHAR_AT(5)
												CHAR('d')
													ON_LEAF_NOBR(6, "\xe2\x8c\x85");  // &barwed;
													ON_SUFFIX(6, "ge", "\xe2\x8c\x85");  // &barwedge;
											CHAR_END()
									CHAR_END()
							CHAR_END()
					CHAR_END()
				CHAR('b')
					CHAR_AT(2)
						CHAR('r')
							CHAR_AT(3)
								CHAR('k')
									ON_LEAF_NOBR(4, "\xe2\x8e\xb5");  // &bbrk;
									ON_SUFFIX(4, "tbrk", "\xe2\x8e\xb6");  // &bbrktbrk;
							CHAR_END()
					CHAR_END()
				CHAR('c')
					CHAR_AT(2)
						CHAR('o') ON_SUFFIX(3, "ng", "\xe2\x89\x8c");  // &bcong;
						CHAR('y') ON_LEAF(3, "\xd0\xb1");  // &bcy;
					CHAR_END()
				CHAR('d') ON_SUFFIX(2, "quo", "\xe2\x80\x9e");  // &bdquo;
				CHAR('e')
					CHAR_AT(2)
						CHAR('c')
							CHAR_AT(3)
								CHAR('a')
									CHAR_AT(4)
										CHAR('u')
											CHAR_AT(5)
												CHAR('s')
													ON_LEAF_NOBR(6, "\xe2\x88\xb5");  // &becaus;
													ON_SUFFIX(6, "e", "\xe2\x88\xb5");  // &because;
											CHAR_END()
									CHAR_END()
							CHAR_END()
						CHAR('m') ON_SUFFIX(3, "ptyv", "\xe2\xa6\xb0");  // &bemptyv;
						CHAR('p') ON_SUFFIX(3, "si", "\xcf\xb6");  // &bepsi;
						CHAR('r') ON_SUFFIX(3, "nou", "\xe2\x84\xac");  // &bernou;
						CHAR('t')
							CHAR_AT(3)
								CHAR('a') ON_LEAF(4, "\xce\xb2");  // &beta;
								CHAR('h') ON_LEAF(4, "\xe2\x84\xb6");  // &beth;
								CHAR('w') ON_SUFFIX(4, "een", "\xe2\x89\xac");  // &between;
							CHAR_END()
					CHAR_END()
				CHAR('f') ON_SUFFIX(2, "r", "\xf0\x9d\x94\x9f");  // &bfr;
				CHAR('i')
					CHAR_AT(2)
						CHAR('g')
							CHAR_AT(3)
								CHAR('c')
									CHAR_AT(4)
										CHAR('a') ON_SUFFIX(5, "p", "\xe2\x8b\x82");  // &bigcap;
										CHAR('i') ON_SUFFIX(5, "rc", "\xe2\x97\xaf");  // &bigcirc;
										CHAR('u') ON_SUFFIX(5, "p", "\xe2\x8b\x83");  // &bigcup;
									CHAR_END()
								CHAR('o')
									CHAR_AT(4)
										CHAR('d') ON_SUFFIX(5, "ot", "\xe2\xa8\x80");  // &bigodot;
										CHAR('p') ON_SUFFIX(5, "lus", "\xe2\xa8\x81");  // &bigoplus;
										CHAR('t') ON_SUFFIX(5, "imes", "\xe2\xa8\x82");  // &bigotimes;
									CHAR_END()
								CHAR('s')
									CHAR_AT(4)
										CHAR('q') ON_SUFFIX(5, "cup", "\xe2\xa8\x86");  // &bigsqcup;
										CHAR('t') ON_SUFFIX(5, "ar", "\xe2\x98\x85");  // &bigstar;
									CHAR_END()
								CHAR('t')
									CHAR_AT(4)
										CHAR('r')
											CHAR_AT(5)
												CHAR('i')
													CHAR_AT(6)
														CHAR('a')
															CHAR_AT(7)
																CHAR('n')
																	CHAR_AT(8)
																		CHAR('g')
																			CHAR_AT(9)
																				CHAR('l')
																					CHAR_AT(10)
																						CHAR('e')
																							CHAR_AT(11)
																								CHAR('d') ON_SUFFIX(12, "own", "\xe2\x96\xbd");  // &bigtriangledown;
																								CHAR('u') ON_SUFFIX(12, "p", "\xe2\x96\xb3");  // &bigtriangleup;
																							CHAR_END()
																					CHAR_END()
																			CHAR_END()
																	CHAR_END()
															CHAR_END()
													CHAR_END()
											CHAR_END()
									CHAR_END()
								CHAR('u') ON_SUFFIX(4, "plus", "\xe2\xa8\x84");  // &biguplus;
								CHAR('v') ON_SUFFIX(4, "ee", "\xe2\x8b\x81");  // &bigvee;
								CHAR('w') ON_SUFFIX(4, "edge", "\xe2\x8b\x80");  // &bigwedge;
							CHAR_END()
					CHAR_END()
				CHAR('k') ON_SUFFIX(2, "arow", "\xe2\xa4\x8d");  // &bkarow;
				CHAR('l')
					CHAR_AT(2)
						CHAR('a')
							CHAR_AT(3)
								CHAR('c')
									CHAR_AT(4)
										CHAR('k')
											CHAR_AT(5)
												CHAR('l') ON_SUFFIX(6, "ozenge", "\xe2\xa7\xab");  // &blacklozenge;
												CHAR('s') ON_SUFFIX(6, "quare", "\xe2\x96\xaa");  // &blacksquare;
												CHAR('t')
													CHAR_AT(6)
														CHAR('r')
															CHAR_AT(7)
																CHAR('i')
																	CHAR_AT(8)
																		CHAR('a')
																			CHAR_AT(9)
																				CHAR('n')
																					CHAR_AT(10)
																						CHAR('g')
																							CHAR_AT(11)
																								CHAR('l')
																									CHAR_AT(12)
																										CHAR('e')
																											ON_LEAF_NOBR(13, "\xe2\x96\xb4");  // &blacktriangle;
																											CHAR_AT(13)
																												CHAR('d') ON_SUFFIX(14, "own", "\xe2\x96\xbe");  // &blacktriangledown;
																												CHAR('l') ON_SUFFIX(14, "eft", "\xe2\x97\x82");  // &blacktriangleleft;
																												CHAR('r') ON_SUFFIX(14, "ight", "\xe2\x96\xb8");  // &blacktriangleright;
																											CHAR_END()
																									CHAR_END()
																							CHAR_END()
																					CHAR_END()
																			CHAR_END()
																	CHAR_END()
															CHAR_END()
													CHAR_END()
											CHAR_END()
									CHAR_END()
								CHAR('n') ON_SUFFIX(4, "k", "\xe2\x90\xa3");  // &blank;
							CHAR_END()
						CHAR('k')
							CHAR_AT(3)
								CHAR('1')
									CHAR_AT(4)
										CHAR('2') ON_LEAF(5, "\xe2\x96\x92");  // &blk12;
										CHAR('4') ON_LEAF(5, "\xe2\x96\x91");  // &blk14;
									CHAR_END()
								CHAR('3') ON_SUFFIX(4, "4", "\xe2\x96\x93");  // &blk34;
							CHAR_END()
						CHAR('o') ON_SUFFIX(3, "ck", "\xe2\x96\x88");  // &block;
					CHAR_END()
				CHAR('n')
					CHAR_AT(2)
						CHAR('e')
							ON_LEAF_NOBR(3, "=\xe2\x83\xa5");  // &bne;
							ON_SUFFIX(3, "quiv", "\xe2\x89\xa1\xe2\x83\xa5");  // &bnequiv;
						CHAR('o') ON_SUFFIX(3, "t", "\xe2\x8c\x90");  // &bnot;
					CHAR_END()
				CHAR('o')
					CHAR_AT(2)
						CHAR('p') ON_SUFFIX(3, "f", "\xf0\x9d\x95\x93");  // &bopf;
						CHAR('t')
							ON_LEAF_NOBR(3, "\xe2\x8a\xa5");  // &bot;
							ON_SUFFIX(3, "tom", "\xe2\x8a\xa5");  // &bottom;
						CHAR('w') ON_SUFFIX(3, "tie", "\xe2\x8b\x88");  // &bowtie;
						CHAR('x')
							CHAR_AT(3)
								CHAR('D')
									CHAR_AT(4)
										CHAR('L') ON_LEAF(5, "\xe2\x95\x97");  // &boxDL;
										CHAR('R') ON_LEAF(5, "\xe2\x95\x94");  // &boxDR;
										CHAR('l') ON_LEAF(5, "\xe2\x95\x96");  // &boxDl;
										CHAR('r') ON_LEAF(5, "\xe2\x95\x93");  // &boxDr;
									CHAR_END()
								CHAR('H')
									ON_LEAF_NOBR(4, "\xe2\x95\x90");  // &boxH;
									CHAR_AT(4)
										CHAR('D') ON_LEAF(5, "\xe2\x95\xa6");  // &boxHD;
										CHAR('U') ON_LEAF(5, "\xe2\x95\xa9");  // &boxHU;
										CHAR('d') ON_LEAF(5, "\xe2\x95\xa4");  // &boxHd;
										CHAR('u') ON_LEAF(5, "\xe2\x95\xa7");  // &boxHu;
									CHAR_END()
								CHAR('U')
									CHAR_AT(4)
										CHAR('L') ON_LEAF(5, "\xe2\x95\x9d");  // &boxUL;
										CHAR('R') ON_LEAF(5, "\xe2\x95\x9a");  // &boxUR;
										CHAR('l') ON_LEAF(5, "\xe2\x95\x9c");  // &boxUl;
										CHAR('r') ON_LEAF(5, "\xe2\x95\x99");  // &boxUr;
									CHAR_END()
								CHAR('V')
									ON_LEAF_NOBR(4, "\xe2\x95\x91");  // &boxV;
									CHAR_AT(4)
										CHAR('H') ON_LEAF(5, "\xe2\x95\xac");  // &boxVH;
										CHAR('L') ON_LEAF(5, "\xe2\x95\xa3");  // &boxVL;
										CHAR('R') ON_LEAF(5, "\xe2\x95\xa0");  // &boxVR;
										CHAR('h') ON_LEAF(5, "\xe2\x95\xab");  // &boxVh;
										CHAR('l') ON_LEAF(5, "\xe2\x95\xa2");  // &boxVl;
										CHAR('r') ON_LEAF(5, "\xe2\x95\x9f");  // &boxVr;
									CHAR_END()
								CHAR('b') ON_SUFFIX(4, "ox", "\xe2\xa7\x89");  // &boxbox;
								CHAR('d')
									CHAR_AT(4)
										CHAR('L') ON_LEAF(5, "\xe2\x95\x95");  // &boxdL;
										CHAR('R') ON_LEAF(5, "\xe2\x95\x92");  // &boxdR;
										CHAR('l') ON_LEAF(5, "\xe2\x94\x90");  // &boxdl;
										CHAR('r') ON_LEAF(5, "\xe2\x94\x8c");  // &boxdr;
									CHAR_END()
								CHAR('h')
									ON_LEAF_NOBR(4, "\xe2\x94\x80");  // &boxh;
									CHAR_AT(4)
										CHAR('D') ON_LEAF(5, "\xe2\x95\xa5");  // &boxhD;
										CHAR('U') ON_LEAF(5, "\xe2\x95\xa8");  // &boxhU;
										CHAR('d') ON_LEAF(5, "\xe2\x94\xac");  // &boxhd;
										CHAR('u') ON_LEAF(5, "\xe2\x94\xb4");  // &boxhu;
									CHAR_END()
								CHAR('m') ON_SUFFIX(4, "inus", "\xe2\x8a\x9f");  // &boxminus;
								CHAR('p') ON_SUFFIX(4, "lus", "\xe2\x8a\x9e");  // &boxplus;
								CHAR('t') ON_SUFFIX(4, "imes", "\xe2\x8a\xa0");  // &boxtimes;
								CHAR('u')
									CHAR_AT(4)
										CHAR('L') ON_LEAF(5, "\xe2\x95\x9b");  // &boxuL;
										CHAR('R') ON_LEAF(5, "\xe2\x95\x98");  // &boxuR;
										CHAR('l') ON_LEAF(5, "\xe2\x94\x98");  // &boxul;
										CHAR('r') ON_LEAF(5, "\xe2\x94\x94");  // &boxur;
									CHAR_END()
								CHAR('v')
									ON_LEAF_NOBR(4, "\xe2\x94\x82");  // &boxv;
									CHAR_AT(4)
										CHAR('H') ON_LEAF(5, "\xe2\x95\xaa");  // &boxvH;
										CHAR('L') ON_LEAF(5, "\xe2\x95\xa1");  // &boxvL;
										CHAR('R') ON_LEAF(5, "\xe2\x95\x9e");  // &boxvR;
										CHAR('h') ON_LEAF(5, "\xe2\x94\xbc");  // &boxvh;
										CHAR('l') ON_LEAF(5, "\xe2\x94\xa4");  // &boxvl;
										CHAR('r') ON_LEAF(5, "\xe2\x94\x9c");  // &boxvr;
									CHAR_END()
							CHAR_END()
					CHAR_END()
				CHAR('p') ON_SUFFIX(2, "rime", "\xe2\x80\xb5");  // &bprime;
				CHAR('r')
					CHAR_AT(2)
						CHAR('e') ON_SUFFIX(3, "ve", "\xcb\x98");  // &breve;
						CHAR('v') ON_SUFFIX(3, "bar", "\xc2\xa6");  // &brvbar;
					CHAR_END()
				CHAR('s')
					CHAR_AT(2)
						CHAR('c') ON_SUFFIX(3, "r", "\xf0\x9d\x92\xb7");  // &bscr;
						CHAR('e') ON_SUFFIX(3, "mi", "\xe2\x81\x8f");  // &bsemi;
						CHAR('i')
							CHAR_AT(3)
								CHAR('m')
									ON_LEAF_NOBR(4, "\xe2\x88\xbd");  // &bsim;
									ON_SUFFIX(4, "e", "\xe2\x8b\x8d");  // &bsime;
							CHAR_END()
						CHAR('o')
							CHAR_AT(3)
								CHAR('l')
									ON_LEAF_NOBR(4, "\\");  // &bsol;
									CHAR_AT(4)
										CHAR('b') ON_LEAF(5, "\xe2\xa7\x85");  // &bsolb;
										CHAR('h') ON_SUFFIX(5, "sub", "\xe2\x9f\x88");  // &bsolhsub;
									CHAR_END()
							CHAR_END()
					CHAR_END()
				CHAR('u')
					CHAR_AT(2)
						CHAR('l')
							CHAR_AT(3)
								CHAR('l')
									ON_LEAF_NOBR(4, "\xe2\x80\xa2");  // &bull;
									ON_SUFFIX(4, "et", "\xe2\x80\xa2");  // &bullet;
							CHAR_END()
						CHAR('m')
							CHAR_AT(3)
								CHAR('p')
									ON_LEAF_NOBR(4, "\xe2\x89\x8e");  // &bump;
									CHAR_AT(4)
										CHAR('E') ON_LEAF(5, "\xe2\xaa\xae");  // &bumpE;
										CHAR('e')
											ON_LEAF_NOBR(5, "\xe2\x89\x8f");  // &bumpe;
											ON_SUFFIX(5, "q", "\xe2\x89\x8f");  // &bumpeq;
									CHAR_END()
							CHAR_END()
					CHAR_END()
			CHAR_END()
		CHAR('c')
			CHAR_AT(1)
				CHAR('a')
					CHAR_AT(2)
						CHAR('c') ON_SUFFIX(3, "ute", "\xc4\x87");  // &cacute;
						CHAR('p')
							ON_LEAF_NOBR(3, "\xe2\x88\xa9");  // &cap;
							CHAR_AT(3)
								CHAR('a') ON_SUFFIX(4, "nd", "\xe2\xa9\x84");  // &capand;
								CHAR('b') ON_SUFFIX(4, "rcup", "\xe2\xa9\x89");  // &capbrcup;
								CHAR('c')
									CHAR_AT(4)
										CHAR('a') ON_SUFFIX(5, "p", "\xe2\xa9\x8b");  // &capcap;
										CHAR('u') ON_SUFFIX(5, "p", "\xe2\xa9\x87");  // &capcup;
									CHAR_END()
								CHAR('d') ON_SUFFIX(4, "ot", "\xe2\xa9\x80");  // &capdot;
								CHAR('s') ON_LEAF(4, "\xe2\x88\xa9\xef\xb8\x80");  // &caps;
							CHAR_END()
						CHAR('r')
							CHAR_AT(3)
								CHAR('e') ON_SUFFIX(4, "t", "\xe2\x81\x81");  // &caret;
								CHAR('o') ON_SUFFIX(4, "n", "\xcb\x87");  // &caron;
							CHAR_END()
					CHAR_END()
				CHAR('c')
					CHAR_AT(2)
						CHAR('a')
							CHAR_AT(3)
								CHAR('p') ON_SUFFIX(4, "s", "\xe2\xa9\x8d");  // &ccaps;
								CHAR('r') ON_SUFFIX(4, "on", "\xc4\x8d");  // &ccaron;
							CHAR_END()
						CHAR('e') ON_SUFFIX(3, "dil", "\xc3\xa7");  // &ccedil;
						CHAR('i') ON_SUFFIX(3, "rc", "\xc4\x89");  // &ccirc;
						CHAR('u')
							CHAR_AT(3)
								CHAR('p')
									CHAR_AT(4)
										CHAR('s')
											ON_LEAF_NOBR(5, "\xe2\xa9\x8c");  // &ccups;
											ON_SUFFIX(5, "sm", "\xe2\xa9\x90");  // &ccupssm;
									CHAR_END()
							CHAR_END()
					CHAR_END()
				CHAR('d') ON_SUFFIX(2, "ot", "\xc4\x8b");  // &cdot;
				CHAR('e')
					CHAR_AT(2)
						CHAR('d') ON_SUFFIX(3, "il", "\xc2\xb8");  // &cedil;
						CHAR('m') ON_SUFFIX(3, "ptyv", "\xe2\xa6\xb2");  // &cemptyv;
						CHAR('n')
							CHAR_AT(3)
								CHAR('t')
									ON_LEAF_NOBR(4, "\xc2\xa2");  // &cent;
									ON_SUFFIX(4, "erdot", "\xc2\xb7");  // &centerdot;
							CHAR_END()
					CHAR_END()
				CHAR('f') ON_SUFFIX(2, "r", "\xf0\x9d\x94\xa0");  // &cfr;
				CHAR('h')
					CHAR_AT(2)
						CHAR('c') ON_SUFFIX(3, "y", "\xd1\x87");  // &chcy;
						CHAR('e')
							CHAR_AT(3)
								CHAR('c')
									CHAR_AT(4)
										CHAR('k')
											ON_LEAF_NOBR(5, "\xe2\x9c\x93");  // &check;
											ON_SUFFIX(5, "mark", "\xe2\x9c\x93");  // &checkmark;
									CHAR_END()
							CHAR_END()
						CHAR('i') ON_LEAF(3, "\xcf\x87");  // &chi;
					CHAR_END()
				CHAR('i')
					CHAR_AT(2)
						CHAR('r')
							ON_LEAF_NOBR(3, "\xe2\x97\x8b");  // &cir;
							CHAR_AT(3)
								CHAR('E') ON_LEAF(4, "\xe2\xa7\x83");  // &cirE;
								CHAR('c')
									ON_LEAF_NOBR(4, "\xcb\x86");  // &circ;
									CHAR_AT(4)
										CHAR('e') ON_SUFFIX(5, "q", "\xe2\x89\x97");  // &circeq;
										CHAR('l')
											CHAR_AT(5)
												CHAR('e')
													CHAR_AT(6)
														CHAR('a')
															CHAR_AT(7)
																CHAR('r')
																	CHAR_AT(8)
																		CHAR('r')
																			CHAR_AT(9)
																				CHAR('o')
																					CHAR_AT(10)
																						CHAR('w')
																							CHAR_AT(11)
																								CHAR('l') ON_SUFFIX(12, "eft", "\xe2\x86\xba");  // &circlearrowleft;
																								CHAR('r') ON_SUFFIX(12, "ight", "\xe2\x86\xbb");  // &circlearrowright;
																							CHAR_END()
																					CHAR_END()
																			CHAR_END()
																	CHAR_END()
															CHAR_END()
														CHAR('d')
															CHAR_AT(7)
																CHAR('R') ON_LEAF(8, "\xc2\xae");  // &circledR;
																CHAR('S') ON_LEAF(8, "\xe2\x93\x88");  // &circledS;
																CHAR('a') ON_SUFFIX(8, "st", "\xe2\x8a\x9b");  // &circledast;
																CHAR('c') ON_SUFFIX(8, "irc", "\xe2\x8a\x9a");  // &circledcirc;
																CHAR('d') ON_SUFFIX(8, "ash", "\xe2\x8a\x9d");  // &circleddash;
															CHAR_END()
													CHAR_END()
											CHAR_END()
									CHAR_END()
								CHAR('e') ON_LEAF(4, "\xe2\x89\x97");  // &cire;
								CHAR('f') ON_SUFFIX(4, "nint", "\xe2\xa8\x90");  // &cirfnint;
								CHAR('m') ON_SUFFIX(4, "id", "\xe2\xab\xaf");  // &cirmid;
								CHAR('s') ON_SUFFIX(4, "cir", "\xe2\xa7\x82");  // &cirscir;
							CHAR_END()
					CHAR_END()
				CHAR('l')
					CHAR_AT(2)
						CHAR('u')
							CHAR_AT(3)
								CHAR('b')
									CHAR_AT(4)
										CHAR('s')
											ON_LEAF_NOBR(5, "\xe2\x99\xa3");  // &clubs;
											ON_SUFFIX(5, "uit", "\xe2\x99\xa3");  // &clubsuit;
									CHAR_END()
							CHAR_END()
					CHAR_END()
				CHAR('o')
					CHAR_AT(2)
						CHAR('l')
							CHAR_AT(3)
								CHAR('o')
									CHAR_AT(4)
										CHAR('n')
											ON_LEAF_NOBR(5, ":");  // &colon;
											CHAR_AT(5)
												CHAR('e')
													ON_LEAF_NOBR(6, "\xe2\x89\x94");  // &colone;
													ON_SUFFIX(6, "q", "\xe2\x89\x94");  // &coloneq;
											CHAR_END()
									CHAR_END()
							CHAR_END()
						CHAR('m')
							CHAR_AT(3)
								CHAR('m')
									CHAR_AT(4)
										CHAR('a')
											ON_LEAF_NOBR(5, ",");  // &comma;
											ON_SUFFIX(5, "t", "@");  // &commat;
									CHAR_END()
								CHAR('p')
									ON_LEAF_NOBR(4, "\xe2\x88\x81");  // &comp;
									CHAR_AT(4)
										CHAR('f') ON_SUFFIX(5, "n", "\xe2\x88\x98");  // &compfn;
										CHAR('l')
											CHAR_AT(5)
												CHAR('e')
													CHAR_AT(6)
														CHAR('m') ON_SUFFIX(7, "ent", "\xe2\x88\x81");  // &complement;
														CHAR('x') ON_SUFFIX(7, "es", "\xe2\x84\x82");  // &complexes;
													CHAR_END()
											CHAR_END()
									CHAR_END()
							CHAR_END()
						CHAR('n')
							CHAR_AT(3)
								CHAR('g')
									ON_LEAF_NOBR(4, "\xe2\x89\x85");  // &cong;
									ON_SUFFIX(4, "dot", "\xe2\xa9\xad");  // &congdot;
								CHAR('i') ON_SUFFIX(4, "nt", "\xe2\x88\xae");  // &conint;
							CHAR_END()
						CHAR('p')
							CHAR_AT(3)
								CHAR('f') ON_LEAF(4, "\xf0\x9d\x95\x94");  // &copf;
								CHAR('r') ON_SUFFIX(4, "od", "\xe2\x88\x90");  // &coprod;
								CHAR('y')
									ON_LEAF_NOBR(4, "\xc2\xa9");  // &copy;
									ON_SUFFIX(4, "sr", "\xe2\x84\x97");  // &copysr;
							CHAR_END()
					CHAR_END()
				CHAR('r')
					CHAR_AT(2)
						CHAR('a') ON_SUFFIX(3, "rr", "\xe2\x86\xb5");  // &crarr;
						CHAR('o') ON_SUFFIX(3, "ss", "\xe2\x9c\x97");  // &cross;
					CHAR_END()
				CHAR('s')
					CHAR_AT(2)
						CHAR('c') ON_SUFFIX(3, "r", "\xf0\x9d\x92\xb8");  // &cscr;
						CHAR('u')
							CHAR_AT(3)
								CHAR('b')
									ON_LEAF_NOBR(4, "\xe2\xab\x8f");  // &csub;
									ON_SUFFIX(4, "e", "\xe2\xab\x91");  // &csube;
								CHAR('p')
									ON_LEAF_NOBR(4, "\xe2\xab\x90");  // &csup;
									ON_SUFFIX(4, "e", "\xe2\xab\x92");  // &csupe;
							CHAR_END()
					CHAR_END()
				CHAR('t') ON_SUFFIX(2, "dot", "\xe2\x8b\xaf");  // &ctdot;
				CHAR('u')
					CHAR_AT(2)
						CHAR('d')
							CHAR_AT(3)
								CHAR('a')
									CHAR_AT(4)
										CHAR('r')
											CHAR_AT(5)
												CHAR('r')
													CHAR_AT(6)
														CHAR('l') ON_LEAF(7, "\xe2\xa4\xb8");  // &cudarrl;
														CHAR('r') ON_LEAF(7, "\xe2\xa4\xb5");  // &cudarrr;
													CHAR_END()
											CHAR_END()
									CHAR_END()
							CHAR_END()
						CHAR('e')
							CHAR_AT(3)
								CHAR('p') ON_SUFFIX(4, "r", "\xe2\x8b\x9e");  // &cuepr;
								CHAR('s') ON_SUFFIX(4, "c", "\xe2\x8b\x9f");  // &cuesc;
							CHAR_END()
						CHAR('l')
							CHAR_AT(3)
								CHAR('a')
									CHAR_AT(4)
										CHAR('r')
											CHAR_AT(5)
												CHAR('r')
													ON_LEAF_NOBR(6, "\xe2\x86\xb6");  // &cularr;
													ON_SUFFIX(6, "p", "\xe2\xa4\xbd");  // &cularrp;
											CHAR_END()
									CHAR_END()
							CHAR_END()
						CHAR('p')
							ON_LEAF_NOBR(3, "\xe2\x88\xaa");  // &cup;
							CHAR_AT(3)
								CHAR('b') ON_SUFFIX(4, "rcap", "\xe2\xa9\x88");  // &cupbrcap;
								CHAR('c')
									CHAR_AT(4)
										CHAR('a') ON_SUFFIX(5, "p", "\xe2\xa9\x86");  // &cupcap;
										CHAR('u') ON_SUFFIX(5, "p", "\xe2\xa9\x8a");  // &cupcup;
									CHAR_END()
								CHAR('d') ON_SUFFIX(4, "ot", "\xe2\x8a\x8d");  // &cupdot;
								CHAR('o') ON_SUFFIX(4, "r", "\xe2\xa9\x85");  // &cupor;
								CHAR('s') ON_LEAF(4, "\xe2\x88\xaa\xef\xb8\x80");  // &cups;
							CHAR_END()
						CHAR('r')
							CHAR_AT(3)
								CHAR('a')
									CHAR_AT(4)
										CHAR('r')
											CHAR_AT(5)
												CHAR('r')
													ON_LEAF_NOBR(6, "\xe2\x86\xb7");  // &curarr;
													ON_SUFFIX(6, "m", "\xe2\xa4\xbc");  // &curarrm;
											CHAR_END()
									CHAR_END()
								CHAR('l')
									CHAR_AT(4)
										CHAR('y')
											CHAR_AT(5)
												CHAR('e')
													CHAR_AT(6)
														CHAR('q')
															CHAR_AT(7)
																CHAR('p') ON_SUFFIX(8, "rec", "\xe2\x8b\x9e");  // &curlyeqprec;
																CHAR('s') ON_SUFFIX(8, "ucc", "\xe2\x8b\x9f");  // &curlyeqsucc;
															CHAR_END()
													CHAR_END()
												CHAR('v') ON_SUFFIX(6, "ee", "\xe2\x8b\x8e");  // &curlyvee;
												CHAR('w') ON_SUFFIX(6, "edge", "\xe2\x8b\x8f");  // &curlywedge;
											CHAR_END()
									CHAR_END()
								CHAR('r') ON_SUFFIX(4, "en", "\xc2\xa4");  // &curren;
								CHAR('v')
									CHAR_AT(4)
										CHAR('e')
											CHAR_AT(5)
												CHAR('a')
													CHAR_AT(6)
														CHAR('r')
															CHAR_AT(7)
																CHAR('r')
																	CHAR_AT(8)
																		CHAR('o')
																			CHAR_AT(9)
																				CHAR('w')
																					CHAR_AT(10)
																						CHAR('l') ON_SUFFIX(11, "eft", "\xe2\x86\xb6");  // &curvearrowleft;
																						CHAR('r') ON_SUFFIX(11, "ight", "\xe2\x86\xb7");  // &curvearrowright;
																					CHAR_END()
																			CHAR_END()
																	CHAR_END()
															CHAR_END()
													CHAR_END()
											CHAR_END()
									CHAR_END()
							CHAR_END()
						CHAR('v') ON_SUFFIX(3, "ee", "\xe2\x8b\x8e");  // &cuvee;
						CHAR('w') ON_SUFFIX(3, "ed", "\xe2\x8b\x8f");  // &cuwed;
					CHAR_END()
				CHAR('w')
					CHAR_AT(2)
						CHAR('c') ON_SUFFIX(3, "onint", "\xe2\x88\xb2");  // &cwconint;
						CHAR('i') ON_SUFFIX(3, "nt", "\xe2\x88\xb1");  // &cwint;
					CHAR_END()
				CHAR('y') ON_SUFFIX(2, "lcty", "\xe2\x8c\xad");  // &cylcty;
			CHAR_END()
		CHAR('d')
			CHAR_AT(1)
				CHAR('A') ON_SUFFIX(2, "rr", "\xe2\x87\x93");  // &dArr;
				CHAR('H') ON_SUFFIX(2, "ar", "\xe2\xa5\xa5");  // &dHar;
				CHAR('a')
					CHAR_AT(2)
						CHAR('g') ON_SUFFIX(3, "ger", "\xe2\x80\xa0");  // &dagger;
						CHAR('l') ON_SUFFIX(3, "eth", "\xe2\x84\xb8");  // &daleth;
						CHAR('r') ON_SUFFIX(3, "r", "\xe2\x86\x93");  // &darr;
						CHAR('s')
							CHAR_AT(3)
								CHAR('h')
									ON_LEAF_NOBR(4, "\xe2\x80\x90");  // &dash;
									ON_SUFFIX(4, "v", "\xe2\x8a\xa3");  // &dashv;
							CHAR_END()
					CHAR_END()
				CHAR('b')
					CHAR_AT(2)
						CHAR('k') ON_SUFFIX(3, "arow", "\xe2\xa4\x8f");  // &dbkarow;
						CHAR('l') ON_SUFFIX(3, "ac", "\xcb\x9d");  // &dblac;
					CHAR_END()
				CHAR('c')
					CHAR_AT(2)
						CHAR('a') ON_SUFFIX(3, "ron", "\xc4\x8f");  // &dcaron;
						CHAR('y') ON_LEAF(3, "\xd0\xb4");  // &dcy;
					CHAR_END()
				CHAR('d')
					ON_LEAF_NOBR(2, "\xe2\x85\x86");  // &dd;
					CHAR_AT(2)
						CHAR('a')
							CHAR_AT(3)
								CHAR('g') ON_SUFFIX(4, "ger", "\xe2\x80\xa1");  // &ddagger;
								CHAR('r') ON_SUFFIX(4, "r", "\xe2\x87\x8a");  // &ddarr;
							CHAR_END()
						CHAR('o') ON_SUFFIX(3, "tseq", "\xe2\xa9\xb7");  // &ddotseq;
					CHAR_END()
				CHAR('e')
					CHAR_AT(2)
						CHAR('g') ON_LEAF(3, "\xc2\xb0");  // &deg;
						CHAR('l') ON_SUFFIX(3, "ta", "\xce\xb4");  // &delta;
						CHAR('m') ON_SUFFIX(3, "ptyv", "\xe2\xa6\xb1");  // &demptyv;
					CHAR_END()
				CHAR('f')
					CHAR_AT(2)
						CHAR('i') ON_SUFFIX(3, "sht", "\xe2\xa5\xbf");  // &dfisht;
						CHAR('r') ON_LEAF(3, "\xf0\x9d\x94\xa1");  // &dfr;
					CHAR_END()
				CHAR('h')
					CHAR_AT(2)
						CHAR('a')
							CHAR_AT(3)
								CHAR('r')
									CHAR_AT(4)
										CHAR('l') ON_LEAF(5, "\xe2\x87\x83");  // &dharl;
										CHAR('r') ON_LEAF(5, "\xe2\x87\x82");  // &dharr;
									CHAR_END()
							CHAR_END()
					CHAR_END()
				CHAR('i')
					CHAR_AT(2)
						CHAR('a')
							CHAR_AT(3)
								CHAR('m')
									ON_LEAF_NOBR(4, "\xe2\x8b\x84");  // &diam;
									CHAR_AT(4)
										CHAR('o')
											CHAR_AT(5)
												CHAR('n')
													CHAR_AT(6)
														CHAR('d')
															ON_LEAF_NOBR(7, "\xe2\x8b\x84");  // &diamond;
															ON_SUFFIX(7, "suit", "\xe2\x99\xa6");  // &diamondsuit;
													CHAR_END()
											CHAR_END()
										CHAR('s') ON_LEAF(5, "\xe2\x99\xa6");  // &diams;
									CHAR_END()
							CHAR_END()
						CHAR('e') ON_LEAF(3, "\xc2\xa8");  // &die;
						CHAR('g') ON_SUFFIX(3, "amma", "\xcf\x9d");  // &digamma;
						CHAR('s') ON_SUFFIX(3, "in", "\xe2\x8b\xb2");  // &disin;
						CHAR('v')
							ON_LEAF_NOBR(3, "\xc3\xb7");  // &div;
							CHAR_AT(3)
								CHAR('i')
									CHAR_AT(4)
										CHAR('d')
											CHAR_AT(5)
												CHAR('e')
													ON_LEAF_NOBR(6, "\xc3\xb7");  // &divide;
													ON_SUFFIX(6, "ontimes", "\xe2\x8b\x87");  // &divideontimes;
											CHAR_END()
									CHAR_END()
								CHAR('o') ON_SUFFIX(4, "nx", "\xe2\x8b\x87");  // &divonx;
							CHAR_END()
					CHAR_END()
				CHAR('j') ON_SUFFIX(2, "cy", "\xd1\x92");  // &djcy;
				CHAR('l')
					CHAR_AT(2)
						CHAR('c')
							CHAR_AT(3)
								CHAR('o') ON_SUFFIX(4, "rn", "\xe2\x8c\x9e");  // &dlcorn;
								CHAR('r') ON_SUFFIX(4, "op", "\xe2\x8c\x8d");  // &dlcrop;
							CHAR_END()
					CHAR_END()
				CHAR('o')
					CHAR_AT(2)
						CHAR('l') ON_SUFFIX(3, "lar", "$");  // &dollar;
						CHAR('p') ON_SUFFIX(3, "f", "\xf0\x9d\x95\x95");  // &dopf;
						CHAR('t')
							ON_LEAF_NOBR(3, "\xcb\x99");  // &dot;
							CHAR_AT(3)
								CHAR('e')
									CHAR_AT(4)
										CHAR('q')
											ON_LEAF_NOBR(5, "\xe2\x89\x90");  // &doteq;
											ON_SUFFIX(5, "dot", "\xe2\x89\x91");  // &doteqdot;
									CHAR_END()
								CHAR('m') ON_SUFFIX(4, "inus", "\xe2\x88\xb8");  // &dotminus;
								CHAR('p') ON_SUFFIX(4, "lus", "\xe2\x88\x94");  // &dotplus;
								CHAR('s') ON_SUFFIX(4, "quare", "\xe2\x8a\xa1");  // &dotsquare;
							CHAR_END()
						CHAR('u') ON_SUFFIX(3, "blebarwedge", "\xe2\x8c\x86");  // &doublebarwedge;
						CHAR('w')
							CHAR_AT(3)
								CHAR('n')
									CHAR_AT(4)
										CHAR('a') ON_SUFFIX(5, "rrow", "\xe2\x86\x93");  // &downarrow;
										CHAR('d') ON_SUFFIX(5, "ownarrows", "\xe2\x87\x8a");  // &downdownarrows;
										CHAR('h')
											CHAR_AT(5)
												CHAR('a')
													CHAR_AT(6)
														CHAR('r')
															CHAR_AT(7)
																CHAR('p')
																	CHAR_AT(8)
																		CHAR('o')
																			CHAR_AT(9)
																				CHAR('o')
																					CHAR_AT(10)
																						CHAR('n')
																							CHAR_AT(11)
																								CHAR('l') ON_SUFFIX(12, "eft", "\xe2\x87\x83");  // &downharpoonleft;
																								CHAR('r') ON_SUFFIX(12, "ight", "\xe2\x87\x82");  // &downharpoonright;
																							CHAR_END()
																					CHAR_END()
																			CHAR_END()
																	CHAR_END()
															CHAR_END()
													CHAR_END()
											CHAR_END()
									CHAR_END()
							CHAR_END()
					CHAR_END()
				CHAR('r')
					CHAR_AT(2)
						CHAR('b') ON_SUFFIX(3, "karow", "\xe2\xa4\x90");  // &drbkarow;
						CHAR('c')
							CHAR_AT(3)
								CHAR('o') ON_SUFFIX(4, "rn", "\xe2\x8c\x9f");  // &drcorn;
								CHAR('r') ON_SUFFIX(4, "op", "\xe2\x8c\x8c");  // &drcrop;
							CHAR_END()
					CHAR_END()
				CHAR('s')
					CHAR_AT(2)
						CHAR('c')
							CHAR_AT(3)
								CHAR('r') ON_LEAF(4, "\xf0\x9d\x92\xb9");  // &dscr;
								CHAR('y') ON_LEAF(4, "\xd1\x95");  // &dscy;
							CHAR_END()
						CHAR('o') ON_SUFFIX(3, "l", "\xe2\xa7\xb6");  // &dsol;
						CHAR('t') ON_SUFFIX(3, "rok", "\xc4\x91");  // &dstrok;
					CHAR_END()
				CHAR('t')
					CHAR_AT(2)
						CHAR('d') ON_SUFFIX(3, "ot", "\xe2\x8b\xb1");  // &dtdot;
						CHAR('r')
							CHAR_AT(3)
								CHAR('i')
									ON_LEAF_NOBR(4, "\xe2\x96\xbf");  // &dtri;
									ON_SUFFIX(4, "f", "\xe2\x96\xbe");  // &dtrif;
							CHAR_END()
					CHAR_END()
				CHAR('u')
					CHAR_AT(2)
						CHAR('a') ON_SUFFIX(3, "rr", "\xe2\x87\xb5");  // &duarr;
						CHAR('h') ON_SUFFIX(3, "ar", "\xe2\xa5\xaf");  // &duhar;
					CHAR_END()
				CHAR('w') ON_SUFFIX(2, "angle", "\xe2\xa6\xa6");  // &dwangle;
				CHAR('z')
					CHAR_AT(2)
						CHAR('c') ON_SUFFIX(3, "y", "\xd1\x9f");  // &dzcy;
						CHAR('i') ON_SUFFIX(3, "grarr", "\xe2\x9f\xbf");  // &dzigrarr;
					CHAR_END()
			CHAR_END()
		CHAR('e')
			CHAR_AT(1)
				CHAR('D')
					CHAR_AT(2)
						CHAR('D') ON_SUFFIX(3, "ot", "\xe2\xa9\xb7");  // &eDDot;
						CHAR('o') ON_SUFFIX(3, "t", "\xe2\x89\x91");  // &eDot;
					CHAR_END()
				CHAR('a')
					CHAR_AT(2)
						CHAR('c') ON_SUFFIX(3, "ute", "\xc3\xa9");  // &eacute;
						CHAR('s') ON_SUFFIX(3, "ter", "\xe2\xa9\xae");  // &easter;
					CHAR_END()
				CHAR('c')
					CHAR_AT(2)
						CHAR('a') ON_SUFFIX(3, "ron", "\xc4\x9b");  // &ecaron;
						CHAR('i')
							CHAR_AT(3)
								CHAR('r')
									ON_LEAF_NOBR(4, "\xe2\x89\x96");  // &ecir;
									ON_SUFFIX(4, "c", "\xc3\xaa");  // &ecirc;
							CHAR_END()
						CHAR('o') ON_SUFFIX(3, "lon", "\xe2\x89\x95");  // &ecolon;
						CHAR('y') ON_LEAF(3, "\xd1\x8d");  // &ecy;
					CHAR_END()
				CHAR('d') ON_SUFFIX(2, "ot", "\xc4\x97");  // &edot;
				CHAR('e') ON_LEAF(2, "\xe2\x85\x87");  // &ee;
				CHAR('f')
					CHAR_AT(2)
						CHAR('D') ON_SUFFIX(3, "ot", "\xe2\x89\x92");  // &efDot;
						CHAR('r') ON_LEAF(3, "\xf0\x9d\x94\xa2");  // &efr;
					CHAR_END()
				CHAR('g')
					ON_LEAF_NOBR(2, "\xe2\xaa\x9a");  // &eg;
					CHAR_AT(2)
						CHAR('r') ON_SUFFIX(3, "ave", "\xc3\xa8");  // &egrave;
						CHAR('s')
							ON_LEAF_NOBR(3, "\xe2\xaa\x96");  // &egs;
							ON_SUFFIX(3, "dot", "\xe2\xaa\x98");  // &egsdot;
					CHAR_END()
				CHAR('l')
					ON_LEAF_NOBR(2, "\xe2\xaa\x99");  // &el;
					CHAR_AT(2)
						CHAR('i') ON_SUFFIX(3, "nters", "\xe2\x8f\xa7");  // &elinters;
						CHAR('l') ON_LEAF(3, "\xe2\x84\x93");  // &ell;
						CHAR('s')
							ON_LEAF_NOBR(3, "\xe2\xaa\x95");  // &els;
							ON_SUFFIX(3, "dot", "\xe2\xaa\x97");  // &elsdot;
					CHAR_END()
				CHAR('m')
					CHAR_AT(2)
						CHAR('a') ON_SUFFIX(3, "cr", "\xc4\x93");  // &emacr;
						CHAR('p')
							CHAR_AT(3)
								CHAR('t')
									CHAR_AT(4)
										CHAR('y')
											ON_LEAF_NOBR(5, "\xe2\x88\x85");  // &empty;
											CHAR_AT(5)
												CHAR('s') ON_SUFFIX(6, "et", "\xe2\x88\x85");  // &emptyset;
												CHAR('v') ON_LEAF(6, "\xe2\x88\x85");  // &emptyv;
											CHAR_END()
									CHAR_END()
							CHAR_END()
						CHAR('s')
							CHAR_AT(3)
								CHAR('p')
									ON_LEAF_NOBR(4, "\xe2\x80\x83");  // &emsp;
									CHAR_AT(4)
										CHAR('1')
											CHAR_AT(5)
												CHAR('3') ON_LEAF(6, "\xe2\x80\x84");  // &emsp13;
												CHAR('4') ON_LEAF(6, "\xe2\x80\x85");  // &emsp14;
											CHAR_END()
									CHAR_END()
							CHAR_END()
					CHAR_END()
				CHAR('n')
					CHAR_AT(2)
						CHAR('g') ON_LEAF(3, "\xc5\x8b");  // &eng;
						CHAR('s') ON_SUFFIX(3, "p", "\xe2\x80\x82");  // &ensp;
					CHAR_END()
				CHAR('o')
					CHAR_AT(2)
						CHAR('g') ON_SUFFIX(3, "on", "\xc4\x99");  // &eogon;
						CHAR('p') ON_SUFFIX(3, "f", "\xf0\x9d\x95\x96");  // &eopf;
					CHAR_END()
				CHAR('p')
					CHAR_AT(2)
						CHAR('a')
							CHAR_AT(3)
								CHAR('r')
									ON_LEAF_NOBR(4, "\xe2\x8b\x95");  // &epar;
									ON_SUFFIX(4, "sl", "\xe2\xa7\xa3");  // &eparsl;
							CHAR_END()
						CHAR('l') ON_SUFFIX(3, "us", "\xe2\xa9\xb1");  // &eplus;
						CHAR('s')
							CHAR_AT(3)
								CHAR('i')
									ON_LEAF_NOBR(4, "\xce\xb5");  // &epsi;
									CHAR_AT(4)
										CHAR('l') ON_SUFFIX(5, "on", "\xce\xb5");  // &epsilon;
										CHAR('v') ON_LEAF(5, "\xcf\xb5");  // &epsiv;
									CHAR_END()
							CHAR_END()
					CHAR_END()
				CHAR('q')
					CHAR_AT(2)
						CHAR('c')
							CHAR_AT(3)
								CHAR('i') ON_SUFFIX(4, "rc", "\xe2\x89\x96");  // &eqcirc;
								CHAR('o') ON_SUFFIX(4, "lon", "\xe2\x89\x95");  // &eqcolon;
							CHAR_END()
						CHAR('s')
							CHAR_AT(3)
								CHAR('i') ON_SUFFIX(4, "m", "\xe2\x89\x82");  // &eqsim;
								CHAR('l')
									CHAR_AT(4)
										CHAR('a')
											CHAR_AT(5)
												CHAR('n')
													CHAR_AT(6)
														CHAR('t')
															CHAR_AT(7)
																CHAR('g') ON_SUFFIX(8, "tr", "\xe2\xaa\x96");  // &eqslantgtr;
																CHAR('l') ON_SUFFIX(8, "ess", "\xe2\xaa\x95");  // &eqslantless;
															CHAR_END()
													CHAR_END()
											CHAR_END()
									CHAR_END()
							CHAR_END()
						CHAR('u')
							CHAR_AT(3)
								CHAR('a') ON_SUFFIX(4, "ls", "=");  // &equals;
								CHAR('e') ON_SUFFIX(4, "st", "\xe2\x89\x9f");  // &equest;
								CHAR('i')
									CHAR_AT(4)
										CHAR('v')
											ON_LEAF_NOBR(5, "\xe2\x89\xa1");  // &equiv;
											ON_SUFFIX(5, "DD", "\xe2\xa9\xb8");  // &equivDD;
									CHAR_END()
							CHAR_END()
						CHAR('v') ON_SUFFIX(3, "parsl", "\xe2\xa7\xa5");  // &eqvparsl;
					CHAR_END()
				CHAR('r')
					CHAR_AT(2)
						CHAR('D') ON_SUFFIX(3, "ot", "\xe2\x89\x93");  // &erDot;
						CHAR('a') ON_SUFFIX(3, "rr", "\xe2\xa5\xb1");  // &erarr;
					CHAR_END()
				CHAR('s')
					CHAR_AT(2)
						CHAR('c') ON_SUFFIX(3, "r", "\xe2\x84\xaf");  // &escr;
						CHAR('d') ON_SUFFIX(3, "ot", "\xe2\x89\x90");  // &esdot;
						CHAR('i') ON_SUFFIX(3, "m", "\xe2\x89\x82");  // &esim;
					CHAR_END()
				CHAR('t')
					CHAR_AT(2)
						CHAR('a') ON_LEAF(3, "\xce\xb7");  // &eta;
						CHAR('h') ON_LEAF(3, "\xc3\xb0");  // &eth;
					CHAR_END()
				CHAR('u')
					CHAR_AT(2)
						CHAR('m') ON_SUFFIX(3, "l", "\xc3\xab");  // &euml;
						CHAR('r') ON_SUFFIX(3, "o", "\xe2\x82\xac");  // &euro;
					CHAR_END()
				CHAR('x')
					CHAR_AT(2)
						CHAR('c') ON_SUFFIX(3, "l", "!");  // &excl;
						CHAR('i') ON_SUFFIX(3, "st", "\xe2\x88\x83");  // &exist;
						CHAR('p')
							CHAR_AT(3)
								CHAR('e') ON_SUFFIX(4, "ctation", "\xe2\x84\xb0");  // &expectation;
								CHAR('o') ON_SUFFIX(4, "nentiale", "\xe2\x85\x87");  // &exponentiale;
							CHAR_END()
					CHAR_END()
			CHAR_END()
		CHAR('f')
			CHAR_AT(1)
				CHAR('a') ON_SUFFIX(2, "llingdotseq", "\xe2\x89\x92");  // &fallingdotseq;
				CHAR('c') ON_SUFFIX(2, "y", "\xd1\x84");  // &fcy;
				CHAR('e') ON_SUFFIX(2, "male", "\xe2\x99\x80");  // &female;
				CHAR('f')
					CHAR_AT(2)
						CHAR('i') ON_SUFFIX(3, "lig", "\xef\xac\x83");  // &ffilig;
						CHAR('l')
							CHAR_AT(3)
								CHAR('i') ON_SUFFIX(4, "g", "\xef\xac\x80");  // &fflig;
								CHAR('l') ON_SUFFIX(4, "ig", "\xef\xac\x84");  // &ffllig;
							CHAR_END()
						CHAR('r') ON_LEAF(3, "\xf0\x9d\x94\xa3");  // &ffr;
					CHAR_END()
				CHAR('i') ON_SUFFIX(2, "lig", "\xef\xac\x81");  // &filig;
				CHAR('j') ON_SUFFIX(2, "lig", "fj");  // &fjlig;
				CHAR('l')
					CHAR_AT(2)
						CHAR('a') ON_SUFFIX(3, "t", "\xe2\x99\xad");  // &flat;
						CHAR('l') ON_SUFFIX(3, "ig", "\xef\xac\x82");  // &fllig;
						CHAR('t') ON_SUFFIX(3, "ns", "\xe2\x96\xb1");  // &fltns;
					CHAR_END()
				CHAR('n') ON_SUFFIX(2, "of", "\xc6\x92");  // &fnof;
				CHAR('o')
					CHAR_AT(2)
						CHAR('p') ON_SUFFIX(3, "f", "\xf0\x9d\x95\x97");  // &fopf;
						CHAR('r')
							CHAR_AT(3)
								CHAR('a') ON_SUFFIX(4, "ll", "\xe2\x88\x80");  // &forall;
								CHAR('k')
									ON_LEAF_NOBR(4, "\xe2\x8b\x94");  // &fork;
									ON_SUFFIX(4, "v", "\xe2\xab\x99");  // &forkv;
							CHAR_END()
					CHAR_END()
				CHAR('p') ON_SUFFIX(2, "artint", "\xe2\xa8\x8d");  // &fpartint;
				CHAR('r')
					CHAR_AT(2)
						CHAR('a')
							CHAR_AT(3)
								CHAR('c')
									CHAR_AT(4)
										CHAR('1')
											CHAR_AT(5)
												CHAR('2') ON_LEAF(6, "\xc2\xbd");  // &frac12;
												CHAR('3') ON_LEAF(6, "\xe2\x85\x93");  // &frac13;
												CHAR('4') ON_LEAF(6, "\xc2\xbc");  // &frac14;
												CHAR('5') ON_LEAF(6, "\xe2\x85\x95");  // &frac15;
												CHAR('6') ON_LEAF(6, "\xe2\x85\x99");  // &frac16;
												CHAR('8') ON_LEAF(6, "\xe2\x85\x9b");  // &frac18;
											CHAR_END()
										CHAR('2')
											CHAR_AT(5)
												CHAR('3') ON_LEAF(6, "\xe2\x85\x94");  // &frac23;
												CHAR('5') ON_LEAF(6, "\xe2\x85\x96");  // &frac25;
											CHAR_END()
										CHAR('3')
											CHAR_AT(5)
												CHAR('4') ON_LEAF(6, "\xc2\xbe");  // &frac34;
												CHAR('5') ON_LEAF(6, "\xe2\x85\x97");  // &frac35;
												CHAR('8') ON_LEAF(6, "\xe2\x85\x9c");  // &frac38;
											CHAR_END()
										CHAR('4') ON_SUFFIX(5, "5", "\xe2\x85\x98");  // &frac45;
										CHAR('5')
											CHAR_AT(5)
												CHAR('6') ON_LEAF(6, "\xe2\x85\x9a");  // &frac56;
												CHAR('8') ON_LEAF(6, "\xe2\x85\x9d");  // &frac58;
											CHAR_END()
										CHAR('7') ON_SUFFIX(5, "8", "\xe2\x85\x9e");  // &frac78;
									CHAR_END()
								CHAR('s') ON_SUFFIX(4, "l", "\xe2\x81\x84");  // &frasl;
							CHAR_END()
						CHAR('o') ON_SUFFIX(3, "wn", "\xe2\x8c\xa2");  // &frown;
					CHAR_END()
				CHAR('s') ON_SUFFIX(2, "cr", "\xf0\x9d\x92\xbb");  // &fscr;
			CHAR_END()
		CHAR('g')
			CHAR_AT(1)
				CHAR('E')
					ON_LEAF_NOBR(2, "\xe2\x89\xa7");  // &gE;
					ON_SUFFIX(2, "l", "\xe2\xaa\x8c");  // &gEl;
				CHAR('a')
					CHAR_AT(2)
						CHAR('c') ON_SUFFIX(3, "ute", "\xc7\xb5");  // &gacute;
						CHAR('m')
							CHAR_AT(3)
								CHAR('m')
									CHAR_AT(4)
										CHAR('a')
											ON_LEAF_NOBR(5, "\xce\xb3");  // &gamma;
											ON_SUFFIX(5, "d", "\xcf\x9d");  // &gammad;
									CHAR_END()
							CHAR_END()
						CHAR('p') ON_LEAF(3, "\xe2\xaa\x86");  // &gap;
					CHAR_END()
				CHAR('b') ON_SUFFIX(2, "reve", "\xc4\x9f");  // &gbreve;
				CHAR('c')
					CHAR_AT(2)
						CHAR('i') ON_SUFFIX(3, "rc", "\xc4\x9d");  // &gcirc;
						CHAR('y') ON_LEAF(3, "\xd0\xb3");  // &gcy;
					CHAR_END()
				CHAR('d') ON_SUFFIX(2, "ot", "\xc4\xa1");  // &gdot;
				CHAR('e')
					ON_LEAF_NOBR(2, "\xe2\x89\xa5");  // &ge;
					CHAR_AT(2)
						CHAR('l') ON_LEAF(3, "\xe2\x8b\x9b");  // &gel;
						CHAR('q')
							ON_LEAF_NOBR(3, "\xe2\x89\xa5");  // &geq;
							CHAR_AT(3)
								CHAR('q') ON_LEAF(4, "\xe2\x89\xa7");  // &geqq;
								CHAR('s') ON_SUFFIX(4, "lant", "\xe2\xa9\xbe");  // &geqslant;
							CHAR_END()
						CHAR('s')
							ON_LEAF_NOBR(3, "\xe2\xa9\xbe");  // &ges;
							CHAR_AT(3)
								CHAR('c') ON_SUFFIX(4, "c", "\xe2\xaa\xa9");  // &gescc;
								CHAR('d')
									CHAR_AT(4)
										CHAR('o')
											CHAR_AT(5)
												CHAR('t')
													ON_LEAF_NOBR(6, "\xe2\xaa\x80");  // &gesdot;
													CHAR_AT(6)
														CHAR('o')
															ON_LEAF_NOBR(7, "\xe2\xaa\x82");  // &gesdoto;
															ON_SUFFIX(7, "l", "\xe2\xaa\x84");  // &gesdotol;
													CHAR_END()
											CHAR_END()
									CHAR_END()
								CHAR('l')
									ON_LEAF_NOBR(4, "\xe2\x8b\x9b\xef\xb8\x80");  // &gesl;
									ON_SUFFIX(4, "es", "\xe2\xaa\x94");  // &gesles;
							CHAR_END()
					CHAR_END()
				CHAR('f') ON_SUFFIX(2, "r", "\xf0\x9d\x94\xa4");  // &gfr;
				CHAR('g')
					ON_LEAF_NOBR(2, "\xe2\x89\xab");  // &gg;
					ON_SUFFIX(2, "g", "\xe2\x8b\x99");  // &ggg;
				CHAR('i') ON_SUFFIX(2, "mel", "\xe2\x84\xb7");  // &gimel;
				CHAR('j') ON_SUFFIX(2, "cy", "\xd1\x93");  // &gjcy;
				CHAR('l')
					ON_LEAF_NOBR(2, "\xe2\x89\xb7");  // &gl;
					CHAR_AT(2)
						CHAR('E') ON_LEAF(3, "\xe2\xaa\x92");  // &glE;
						CHAR('a') ON_LEAF(3, "\xe2\xaa\xa5");  // &gla;
						CHAR('j') ON_LEAF(3, "\xe2\xaa\xa4");  // &glj;
					CHAR_END()
				CHAR('n')
					CHAR_AT(2)
						CHAR('E') ON_LEAF(3, "\xe2\x89\xa9");  // &gnE;
						CHAR('a')
							CHAR_AT(3)
								CHAR('p')
									ON_LEAF_NOBR(4, "\xe2\xaa\x8a");  // &gnap;
									ON_SUFFIX(4, "prox", "\xe2\xaa\x8a");  // &gnapprox;
							CHAR_END()
						CHAR('e')
							ON_LEAF_NOBR(3, "\xe2\xaa\x88");  // &gne;
							CHAR_AT(3)
								CHAR('q')
									ON_LEAF_NOBR(4, "\xe2\xaa\x88");  // &gneq;
									ON_SUFFIX(4, "q", "\xe2\x89\xa9");  // &gneqq;
							CHAR_END()
						CHAR('s') ON_SUFFIX(3, "im", "\xe2\x8b\xa7");  // &gnsim;
					CHAR_END()
				CHAR('o') ON_SUFFIX(2, "pf", "\xf0\x9d\x95\x98");  // &gopf;
				CHAR('r') ON_SUFFIX(2, "ave", "`");  // &grave;
				CHAR('s')
					CHAR_AT(2)
						CHAR('c') ON_SUFFIX(3, "r", "\xe2\x84\x8a");  // &gscr;
						CHAR('i')
							CHAR_AT(3)
								CHAR('m')
									ON_LEAF_NOBR(4, "\xe2\x89\xb3");  // &gsim;
									CHAR_AT(4)
										CHAR('e') ON_LEAF(5, "\xe2\xaa\x8e");  // &gsime;
										CHAR('l') ON_LEAF(5, "\xe2\xaa\x90");  // &gsiml;
									CHAR_END()
							CHAR_END()
					CHAR_END()
				CHAR('t')
					ON_LEAF_NOBR(2, ">");  // &gt;
					CHAR_AT(2)
						CHAR('c')
							CHAR_AT(3)
								CHAR('c') ON_LEAF(4, "\xe2\xaa\xa7");  // &gtcc;
								CHAR('i') ON_SUFFIX(4, "r", "\xe2\xa9\xba");  // &gtcir;
							CHAR_END()
						CHAR('d') ON_SUFFIX(3, "ot", "\xe2\x8b\x97");  // &gtdot;
						CHAR('l') ON_SUFFIX(3, "Par", "\xe2\xa6\x95");  // &gtlPar;
						CHAR('q') ON_SUFFIX(3, "uest", "\xe2\xa9\xbc");  // &gtquest;
						CHAR('r')
							CHAR_AT(3)
								CHAR('a')
									CHAR_AT(4)
										CHAR('p') ON_SUFFIX(5, "prox", "\xe2\xaa\x86");  // &gtrapprox;
										CHAR('r') ON_SUFFIX(5, "r", "\xe2\xa5\xb8");  // &gtrarr;
									CHAR_END()
								CHAR('d') ON_SUFFIX(4, "ot", "\xe2\x8b\x97");  // &gtrdot;
								CHAR('e')
									CHAR_AT(4)
										CHAR('q')
											CHAR_AT(5)
												CHAR('l') ON_SUFFIX(6, "ess", "\xe2\x8b\x9b");  // &gtreqless;
												CHAR('q') ON_SUFFIX(6, "less", "\xe2\xaa\x8c");  // &gtreqqless;
											CHAR_END()
									CHAR_END()
								CHAR('l') ON_SUFFIX(4, "ess", "\xe2\x89\xb7");  // &gtrless;
								CHAR('s') ON_SUFFIX(4, "im", "\xe2\x89\xb3");  // &gtrsim;
							CHAR_END()
					CHAR_END()
				CHAR('v')
					CHAR_AT(2)
						CHAR('e') ON_SUFFIX(3, "rtneqq", "\xe2\x89\xa9\xef\xb8\x80");  // &gvertneqq;
						CHAR('n') ON_SUFFIX(3, "E", "\xe2\x89\xa9\xef\xb8\x80");  // &gvnE;
					CHAR_END()
			CHAR_END()
		CHAR('h')
			CHAR_AT(1)
				CHAR('A') ON_SUFFIX(2, "rr", "\xe2\x87\x94");  // &hArr;
				CHAR('a')
					CHAR_AT(2)
						CHAR('i') ON_SUFFIX(3, "rsp", "\xe2\x80\x8a");  // &hairsp;
						CHAR('l') ON_SUFFIX(3, "f", "\xc2\xbd");  // &half;
						CHAR('m') ON_SUFFIX(3, "ilt", "\xe2\x84\x8b");  // &hamilt;
						CHAR('r')
							CHAR_AT(3)
								CHAR('d') ON_SUFFIX(4, "cy", "\xd1\x8a");  // &hardcy;
								CHAR('r')
									ON_LEAF_NOBR(4, "\xe2\x86\x94");  // &harr;
									CHAR_AT(4)
										CHAR('c') ON_SUFFIX(5, "ir", "\xe2\xa5\x88");  // &harrcir;
										CHAR('w') ON_LEAF(5, "\xe2\x86\xad");  // &harrw;
									CHAR_END()
							CHAR_END()
					CHAR_END()
				CHAR('b') ON_SUFFIX(2, "ar", "\xe2\x84\x8f");  // &hbar;
				CHAR('c') ON_SUFFIX(2, "irc", "\xc4\xa5");  // &hcirc;
				CHAR('e')
					CHAR_AT(2)
						CHAR('a')
							CHAR_AT(3)
								CHAR('r')
									CHAR_AT(4)
										CHAR('t')
											CHAR_AT(5)
												CHAR('s')
													ON_LEAF_NOBR(6, "\xe2\x99\xa5");  // &hearts;
													ON_SUFFIX(6, "uit", "\xe2\x99\xa5");  // &heartsuit;
											CHAR_END()
									CHAR_END()
							CHAR_END()
						CHAR('l') ON_SUFFIX(3, "lip", "\xe2\x80\xa6");  // &hellip;
						CHAR('r') ON_SUFFIX(3, "con", "\xe2\x8a\xb9");  // &hercon;
					CHAR_END()
				CHAR('f') ON_SUFFIX(2, "r", "\xf0\x9d\x94\xa5");  // &hfr;
				CHAR('k')
					CHAR_AT(2)
						CHAR('s')
							CHAR_AT(3)
								CHAR('e') ON_SUFFIX(4, "arow", "\xe2\xa4\xa5");  // &hksearow;
								CHAR('w') ON_SUFFIX(4, "arow", "\xe2\xa4\xa6");  // &hkswarow;
							CHAR_END()
					CHAR_END()
				CHAR('o')
					CHAR_AT(2)
						CHAR('a') ON_SUFFIX(3, "rr", "\xe2\x87\xbf");  // &hoarr;
						CHAR('m') ON_SUFFIX(3, "tht", "\xe2\x88\xbb");  // &homtht;
						CHAR('o')
							CHAR_AT(3)
								CHAR('k')
									CHAR_AT(4)
										CHAR('l') ON_SUFFIX(5, "eftarrow", "\xe2\x86\xa9");  // &hookleftarrow;
										CHAR('r') ON_SUFFIX(5, "ightarrow", "\xe2\x86\xaa");  // &hookrightarrow;
									CHAR_END()
							CHAR_END()
						CHAR('p') ON_SUFFIX(3, "f", "\xf0\x9d\x95\x99");  // &hopf;
						CHAR('r') ON_SUFFIX(3, "bar", "\xe2\x80\x95");  // &horbar;
					CHAR_END()
				CHAR('s')
					CHAR_AT(2)
						CHAR('c') ON_SUFFIX(3, "r", "\xf0\x9d\x92\xbd");  // &hscr;
						CHAR('l') ON_SUFFIX(3, "ash", "\xe2\x84\x8f");  // &hslash;
						CHAR('t') ON_SUFFIX(3, "rok", "\xc4\xa7");  // &hstrok;
					CHAR_END()
				CHAR('y')
					CHAR_AT(2)
						CHAR('b') ON_SUFFIX(3, "ull", "\xe2\x81\x83");  // &hybull;
						CHAR('p') ON_SUFFIX(3, "hen", "\xe2\x80\x90");  // &hyphen;
					CHAR_END()
			CHAR_END()
		CHAR('i')
			CHAR_AT(1)
				CHAR('a') ON_SUFFIX(2, "cute", "\xc3\xad");  // &iacute;
				CHAR('c')
					ON_LEAF_NOBR(2, "\xe2\x81\xa3");  // &ic;
					CHAR_AT(2)
						CHAR('i') ON_SUFFIX(3, "rc", "\xc3\xae");  // &icirc;
						CHAR('y') ON_LEAF(3, "\xd0\xb8");  // &icy;
					CHAR_END()
				CHAR('e')
					CHAR_AT(2)
						CHAR('c') ON_SUFFIX(3, "y", "\xd0\xb5");  // &iecy;
						CHAR('x') ON_SUFFIX(3, "cl", "\xc2\xa1");  // &iexcl;
					CHAR_END()
				CHAR('f')
					CHAR_AT(2)
						CHAR('f') ON_LEAF(3, "\xe2\x87\x94");  // &iff;
						CHAR('r') ON_LEAF(3, "\xf0\x9d\x94\xa6");  // &ifr;
					CHAR_END()
				CHAR('g') ON_SUFFIX(2, "rave", "\xc3\xac");  // &igrave;
				CHAR('i')
					ON_LEAF_NOBR(2, "\xe2\x85\x88");  // &ii;
					CHAR_AT(2)
						CHAR('i')
							CHAR_AT(3)
								CHAR('i') ON_SUFFIX(4, "nt", "\xe2\xa8\x8c");  // &iiiint;
								CHAR('n') ON_SUFFIX(4, "t", "\xe2\x88\xad");  // &iiint;
							CHAR_END()
						CHAR('n') ON_SUFFIX(3, "fin", "\xe2\xa7\x9c");  // &iinfin;
						CHAR('o') ON_SUFFIX(3, "ta", "\xe2\x84\xa9");  // &iiota;
					CHAR_END()
				CHAR('j') ON_SUFFIX(2, "lig", "\xc4\xb3");  // &ijlig;
				CHAR('m')
					CHAR_AT(2)
						CHAR('a')
							CHAR_AT(3)
								CHAR('c') ON_SUFFIX(4, "r", "\xc4\xab");  // &imacr;
								CHAR('g')
									CHAR_AT(4)
										CHAR('e') ON_LEAF(5, "\xe2\x84\x91");  // &image;
										CHAR('l') ON_SUFFIX(5, "ine", "\xe2\x84\x90");  // &imagline;
										CHAR('p') ON_SUFFIX(5, "art", "\xe2\x84\x91");  // &imagpart;
									CHAR_END()
								CHAR('t') ON_SUFFIX(4, "h", "\xc4\xb1");  // &imath;
							CHAR_END()
						CHAR('o') ON_SUFFIX(3, "f", "\xe2\x8a\xb7");  // &imof;
						CHAR('p') ON_SUFFIX(3, "ed", "\xc6\xb5");  // &imped;
					CHAR_END()
				CHAR('n')
					ON_LEAF_NOBR(2, "\xe2\x88\x88");  // &in;
					CHAR_AT(2)
						CHAR('c') ON_SUFFIX(3, "are", "\xe2\x84\x85");  // &incare;
						CHAR('f')
							CHAR_AT(3)
								CHAR('i')
									CHAR_AT(4)
										CHAR('n')
											ON_LEAF_NOBR(5, "\xe2\x88\x9e");  // &infin;
											ON_SUFFIX(5, "tie", "\xe2\xa7\x9d");  // &infintie;
									CHAR_END()
							CHAR_END()
						CHAR('o') ON_SUFFIX(3, "dot", "\xc4\xb1");  // &inodot;
						CHAR('t')
							ON_LEAF_NOBR(3, "\xe2\x88\xab");  // &int;
							CHAR_AT(3)
								CHAR('c') ON_SUFFIX(4, "al", "\xe2\x8a\xba");  // &intcal;
								CHAR('e')
									CHAR_AT(4)
										CHAR('g') ON_SUFFIX(5, "ers", "\xe2\x84\xa4");  // &integers;
										CHAR('r') ON_SUFFIX(5, "cal", "\xe2\x8a\xba");  // &intercal;
									CHAR_END()
								CHAR('l') ON_SUFFIX(4, "arhk", "\xe2\xa8\x97");  // &intlarhk;
								CHAR('p') ON_SUFFIX(4, "rod", "\xe2\xa8\xbc");  // &intprod;
							CHAR_END()
					CHAR_END()
				CHAR('o')
					CHAR_AT(2)
						CHAR('c') ON_SUFFIX(3, "y", "\xd1\x91");  // &iocy;
						CHAR('g') ON_SUFFIX(3, "on", "\xc4\xaf");  // &iogon;
						CHAR('p') ON_SUFFIX(3, "f", "\xf0\x9d\x95\x9a");  // &iopf;
						CHAR('t') ON_SUFFIX(3, "a", "\xce\xb9");  // &iota;
					CHAR_END()
				CHAR('p') ON_SUFFIX(2, "rod", "\xe2\xa8\xbc");  // &iprod;
				CHAR('q') ON_SUFFIX(2, "uest", "\xc2\xbf");  // &iquest;
				CHAR('s')
					CHAR_AT(2)
						CHAR('c') ON_SUFFIX(3, "r", "\xf0\x9d\x92\xbe");  // &iscr;
						CHAR('i')
							CHAR_AT(3)
								CHAR('n')
									ON_LEAF_NOBR(4, "\xe2\x88\x88");  // &isin;
									CHAR_AT(4)
										CHAR('E') ON_LEAF(5, "\xe2\x8b\xb9");  // &isinE;
										CHAR('d') ON_SUFFIX(5, "ot", "\xe2\x8b\xb5");  // &isindot;
										CHAR('s')
											ON_LEAF_NOBR(5, "\xe2\x8b\xb4");  // &isins;
											ON_SUFFIX(5, "v", "\xe2\x8b\xb3");  // &isinsv;
										CHAR('v') ON_LEAF(5, "\xe2\x88\x88");  // &isinv;
									CHAR_END()
							CHAR_END()
					CHAR_END()
				CHAR('t')
					ON_LEAF_NOBR(2, "\xe2\x81\xa2");  // &it;
					ON_SUFFIX(2, "ilde", "\xc4\xa9");  // &itilde;
				CHAR('u')
					CHAR_AT(2)
						CHAR('k') ON_SUFFIX(3, "cy", "\xd1\x96");  // &iukcy;
						CHAR('m') ON_SUFFIX(3, "l", "\xc3\xaf");  // &iuml;
					CHAR_END()
			CHAR_END()
		CHAR('j')
			CHAR_AT(1)
				CHAR('c')
					CHAR_AT(2)
						CHAR('i') ON_SUFFIX(3, "rc", "\xc4\xb5");  // &jcirc;
						CHAR('y') ON_LEAF(3, "\xd0\xb9");  // &jcy;
					CHAR_END()
				CHAR('f') ON_SUFFIX(2, "r", "\xf0\x9d\x94\xa7");  // &jfr;
				CHAR('m') ON_SUFFIX(2, "ath", "\xc8\xb7");  // &jmath;
				CHAR('o') ON_SUFFIX(2, "pf", "\xf0\x9d\x95\x9b");  // &jopf;
				CHAR('s')
					CHAR_AT(2)
						CHAR('c') ON_SUFFIX(3, "r", "\xf0\x9d\x92\xbf");  // &jscr;
						CHAR('e') ON_SUFFIX(3, "rcy", "\xd1\x98");  // &jsercy;
					CHAR_END()
				CHAR('u') ON_SUFFIX(2, "kcy", "\xd1\x94");  // &jukcy;
			CHAR_END()
		CHAR('k')
			CHAR_AT(1)
				CHAR('a')
					CHAR_AT(2)
						CHAR('p')
							CHAR_AT(3)
								CHAR('p')
									CHAR_AT(4)
										CHAR('a')
											ON_LEAF_NOBR(5, "\xce\xba");  // &kappa;
											ON_SUFFIX(5, "v", "\xcf\xb0");  // &kappav;
									CHAR_END()
							CHAR_END()
					CHAR_END()
				CHAR('c')
					CHAR_AT(2)
						CHAR('e') ON_SUFFIX(3, "dil", "\xc4\xb7");  // &kcedil;
						CHAR('y') ON_LEAF(3, "\xd0\xba");  // &kcy;
					CHAR_END()
				CHAR('f') ON_SUFFIX(2, "r", "\xf0\x9d\x94\xa8");  // &kfr;
				CHAR('g') ON_SUFFIX(2, "reen", "\xc4\xb8");  // &kgreen;
				CHAR('h') ON_SUFFIX(2, "cy", "\xd1\x85");  // &khcy;
				CHAR('j') ON_SUFFIX(2, "cy", "\xd1\x9c");  // &kjcy;
				CHAR('o') ON_SUFFIX(2, "pf", "\xf0\x9d\x95\x9c");  // &kopf;
				CHAR('s') ON_SUFFIX(2, "cr", "\xf0\x9d\x93\x80");  // &kscr;
			CHAR_END()
		CHAR('l')
			CHAR_AT(1)
				CHAR('A')
					CHAR_AT(2)
						CHAR('a') ON_SUFFIX(3, "rr", "\xe2\x87\x9a");  // &lAarr;
						CHAR('r') ON_SUFFIX(3, "r", "\xe2\x87\x90");  // &lArr;
						CHAR('t') ON_SUFFIX(3, "ail", "\xe2\xa4\x9b");  // &lAtail;
					CHAR_END()
				CHAR('B') ON_SUFFIX(2, "arr", "\xe2\xa4\x8e");  // &lBarr;
				CHAR('E')
					ON_LEAF_NOBR(2, "\xe2\x89\xa6");  // &lE;
					ON_SUFFIX(2, "g", "\xe2\xaa\x8b");  // &lEg;
				CHAR('H') ON_SUFFIX(2, "ar", "\xe2\xa5\xa2");  // &lHar;
				CHAR('a')
					CHAR_AT(2)
						CHAR('c') ON_SUFFIX(3, "ute", "\xc4\xba");  // &lacute;
						CHAR('e') ON_SUFFIX(3, "mptyv", "\xe2\xa6\xb4");  // &laemptyv;
						CHAR('g') ON_SUFFIX(3, "ran", "\xe2\x84\x92");  // &lagran;
						CHAR('m') ON_SUFFIX(3, "bda", "\xce\xbb");  // &lambda;
						CHAR('n')
							CHAR_AT(3)
								CHAR('g')
									ON_LEAF_NOBR(4, "\xe2\x9f\xa8");  // &lang;
									CHAR_AT(4)
										CHAR('d') ON_LEAF(5, "\xe2\xa6\x91");  // &langd;
										CHAR('l') ON_SUFFIX(5, "e", "\xe2\x9f\xa8");  // &langle;
									CHAR_END()
							CHAR_END()
						CHAR('p') ON_LEAF(3, "\xe2\xaa\x85");  // &lap;
						CHAR('q') ON_SUFFIX(3, "uo", "\xc2\xab");  // &laquo;
						CHAR('r')
							CHAR_AT(3)
								CHAR('r')
									ON_LEAF_NOBR(4, "\xe2\x86\x90");  // &larr;
									CHAR_AT(4)
										CHAR('b')
											ON_LEAF_NOBR(5, "\xe2\x87\xa4");  // &larrb;
											ON_SUFFIX(5, "fs", "\xe2\xa4\x9f");  // &larrbfs;
										CHAR('f') ON_SUFFIX(5, "s", "\xe2\xa4\x9d");  // &larrfs;
										CHAR('h') ON_SUFFIX(5, "k", "\xe2\x86\xa9");  // &larrhk;
										CHAR('l') ON_SUFFIX(5, "p", "\xe2\x86\xab");  // &larrlp;
										CHAR('p') ON_SUFFIX(5, "l", "\xe2\xa4\xb9");  // &larrpl;
										CHAR('s') ON_SUFFIX(5, "im", "\xe2\xa5\xb3");  // &larrsim;
										CHAR('t') ON_SUFFIX(5, "l", "\xe2\x86\xa2");  // &larrtl;
									CHAR_END()
							CHAR_END()
						CHAR('t')
							ON_LEAF_NOBR(3, "\xe2\xaa\xab");  // &lat;
							CHAR_AT(3)
								CHAR('a') ON_SUFFIX(4, "il", "\xe2\xa4\x99");  // &latail;
								CHAR('e')
									ON_LEAF_NOBR(4, "\xe2\xaa\xad");  // &late;
									ON_SUFFIX(4, "s", "\xe2\xaa\xad\xef\xb8\x80");  // &lates;
							CHAR_END()
					CHAR_END()
				CHAR('b')
					CHAR_AT(2)
						CHAR('a') ON_SUFFIX(3, "rr", "\xe2\xa4\x8c");  // &lbarr;
						CHAR('b') ON_SUFFIX(3, "rk", "\xe2\x9d\xb2");  // &lbbrk;
						CHAR('r')
							CHAR_AT(3)
								CHAR('a')
									CHAR_AT(4)
										CHAR('c')
											CHAR_AT(5)
												CHAR('e') ON_LEAF(6, "{");  // &lbrace;
												CHAR('k') ON_LEAF(6, "[");  // &lbrack;
											CHAR_END()
									CHAR_END()
								CHAR('k')
									CHAR_AT(4)
										CHAR('e') ON_LEAF(5, "\xe2\xa6\x8b");  // &lbrke;
										CHAR('s')
											CHAR_AT(5)
												CHAR('l')
													CHAR_AT(6)
														CHAR('d') ON_LEAF(7, "\xe2\xa6\x8f");  // &lbrksld;
														CHAR('u') ON_LEAF(7, "\xe2\xa6\x8d");  // &lbrkslu;
													CHAR_END()
											CHAR_END()
									CHAR_END()
							CHAR_END()
					CHAR_END()
				CHAR('c')
					CHAR_AT(2)
						CHAR('a') ON_SUFFIX(3, "ron", "\xc4\xbe");  // &lcaron;
						CHAR('e')
							CHAR_AT(3)
								CHAR('d') ON_SUFFIX(4, "il", "\xc4\xbc");  // &lcedil;
								CHAR('i') ON_SUFFIX(4, "l", "\xe2\x8c\x88");  // &lceil;
							CHAR_END()
						CHAR('u') ON_SUFFIX(3, "b", "{");  // &lcub;
						CHAR('y') ON_LEAF(3, "\xd0\xbb");  // &lcy;
					CHAR_END()
				CHAR('d')
					CHAR_AT(2)
						CHAR('c') ON_SUFFIX(3, "a", "\xe2\xa4\xb6");  // &ldca;
						CHAR('q')
							CHAR_AT(3)
								CHAR('u')
									CHAR_AT(4)
										CHAR('o')
											ON_LEAF_NOBR(5, "\xe2\x80\x9c");  // &ldquo;
											ON_SUFFIX(5, "r", "\xe2\x80\x9e");  // &ldquor;
									CHAR_END()
							CHAR_END()
						CHAR('r')
							CHAR_AT(3)
								CHAR('d') ON_SUFFIX(4, "har", "\xe2\xa5\xa7");  // &ldrdhar;
								CHAR('u') ON_SUFFIX(4, "shar", "\xe2\xa5\x8b");  // &ldrushar;
							CHAR_END()
						CHAR('s') ON_SUFFIX(3, "h", "\xe2\x86\xb2");  // &ldsh;
					CHAR_END()
				CHAR('e')
					ON_LEAF_NOBR(2, "\xe2\x89\xa4");  // &le;
					CHAR_AT(2)
						CHAR('f')
							CHAR_AT(3)
								CHAR('t')
									CHAR_AT(4)
										CHAR('a')
											CHAR_AT(5)
												CHAR('r')
													CHAR_AT(6)
														CHAR('r')
															CHAR_AT(7)
																CHAR('o')
																	CHAR_AT(8)
																		CHAR('w')
																			ON_LEAF_NOBR(9, "\xe2\x86\x90");  // &leftarrow;
																			ON_SUFFIX(9, "tail", "\xe2\x86\xa2");  // &leftarrowtail;
																	CHAR_END()
															CHAR_END()
													CHAR_END()
											CHAR_END()
										CHAR('h')
											CHAR_AT(5)
												CHAR('a')
													CHAR_AT(6)
														CHAR('r')
															CHAR_AT(7)
																CHAR('p')
																	CHAR_AT(8)
																		CHAR('o')
																			CHAR_AT(9)
																				CHAR('o')
																					CHAR_AT(10)
																						CHAR('n')
																							CHAR_AT(11)
																								CHAR('d') ON_SUFFIX(12, "own", "\xe2\x86\xbd");  // &leftharpoondown;
																								CHAR('u') ON_SUFFIX(12, "p", "\xe2\x86\xbc");  // &leftharpoonup;
																							CHAR_END()
																					CHAR_END()
																			CHAR_END()
																	CHAR_END()
															CHAR_END()
													CHAR_END()
											CHAR_END()
										CHAR('l') ON_SUFFIX(5, "eftarrows", "\xe2\x87\x87");  // &leftleftarrows;
										CHAR('r')
											CHAR_AT(5)
												CHAR('i')
													CHAR_AT(6)
														CHAR('g')
															CHAR_AT(7)
																CHAR('h')
																	CHAR_AT(8)
																		CHAR('t')
																			CHAR_AT(9)
																				CHAR('a')
																					CHAR_AT(10)
																						CHAR('r')
																							CHAR_AT(11)
																								CHAR('r')
																									CHAR_AT(12)
																										CHAR('o')
																											CHAR_AT(13)
																												CHAR('w')
																													ON_LEAF_NOBR(14, "\xe2\x86\x94");  // &leftrightarrow;
																													ON_SUFFIX(14, "s", "\xe2\x87\x86");  // &leftrightarrows;
																											CHAR_END()
																									CHAR_END()
																							CHAR_END()
																					CHAR_END()
																				CHAR('h') ON_SUFFIX(10, "arpoons", "\xe2\x87\x8b");  // &leftrightharpoons;
																				CHAR('s') ON_SUFFIX(10, "quigarrow", "\xe2\x86\xad");  // &leftrightsquigarrow;
																			CHAR_END()
																	CHAR_END()
															CHAR_END()
													CHAR_END()
											CHAR_END()
										CHAR('t') ON_SUFFIX(5, "hreetimes", "\xe2\x8b\x8b");  // &leftthreetimes;
									CHAR_END()
							CHAR_END()
						CHAR('g') ON_LEAF(3, "\xe2\x8b\x9a");  // &leg;
						CHAR('q')
							ON_LEAF_NOBR(3, "\xe2\x89\xa4");  // &leq;
							CHAR_AT(3)
								CHAR('q') ON_LEAF(4, "\xe2\x89\xa6");  // &leqq;
								CHAR('s') ON_SUFFIX(4, "lant", "\xe2\xa9\xbd");  // &leqslant;
							CHAR_END()
						CHAR('s')
							ON_LEAF_NOBR(3, "\xe2\xa9\xbd");  // &les;
							CHAR_AT(3)
								CHAR('c') ON_SUFFIX(4, "c", "\xe2\xaa\xa8");  // &lescc;
								CHAR('d')
									CHAR_AT(4)
										CHAR('o')
											CHAR_AT(5)
												CHAR('t')
													ON_LEAF_NOBR(6, "\xe2\xa9\xbf");  // &lesdot;
													CHAR_AT(6)
														CHAR('o')
															ON_LEAF_NOBR(7, "\xe2\xaa\x81");  // &lesdoto;
															ON_SUFFIX(7, "r", "\xe2\xaa\x83");  // &lesdotor;
													CHAR_END()
											CHAR_END()
									CHAR_END()
								CHAR('g')
									ON_LEAF_NOBR(4, "\xe2\x8b\x9a\xef\xb8\x80");  // &lesg;
									ON_SUFFIX(4, "es", "\xe2\xaa\x93");  // &lesges;
								CHAR('s')
									CHAR_AT(4)
										CHAR('a') ON_SUFFIX(5, "pprox", "\xe2\xaa\x85");  // &lessapprox;
										CHAR('d') ON_SUFFIX(5, "ot", "\xe2\x8b\x96");  // &lessdot;
										CHAR('e')
											CHAR_AT(5)
												CHAR('q')
													CHAR_AT(6)
														CHAR('g') ON_SUFFIX(7, "tr", "\xe2\x8b\x9a");  // &lesseqgtr;
														CHAR('q') ON_SUFFIX(7, "gtr", "\xe2\xaa\x8b");  // &lesseqqgtr;
													CHAR_END()
											CHAR_END()
										CHAR('g') ON_SUFFIX(5, "tr", "\xe2\x89\xb6");  // &lessgtr;
										CHAR('s') ON_SUFFIX(5, "im", "\xe2\x89\xb2");  // &lesssim;
									CHAR_END()
							CHAR_END()
					CHAR_END()
				CHAR('f')
					CHAR_AT(2)
						CHAR('i') ON_SUFFIX(3, "sht", "\xe2\xa5\xbc");  // &lfisht;
						CHAR('l') ON_SUFFIX(3, "oor", "\xe2\x8c\x8a");  // &lfloor;
						CHAR('r') ON_LEAF(3, "\xf0\x9d\x94\xa9");  // &lfr;
					CHAR_END()
				CHAR('g')
					ON_LEAF_NOBR(2, "\xe2\x89\xb6");  // &lg;
					ON_SUFFIX(2, "E", "\xe2\xaa\x91");  // &lgE;
				CHAR('h')
					CHAR_AT(2)
						CHAR('a')
							CHAR_AT(3)
								CHAR('r')
									CHAR_AT(4)
										CHAR('d') ON_LEAF(5, "\xe2\x86\xbd");  // &lhard;
										CHAR('u')
											ON_LEAF_NOBR(5, "\xe2\x86\xbc");  // &lharu;
											ON_SUFFIX(5, "l", "\xe2\xa5\xaa");  // &lharul;
									CHAR_END()
							CHAR_END()
						CHAR('b') ON_SUFFIX(3, "lk", "\xe2\x96\x84");  // &lhblk;
					CHAR_END()
				CHAR('j') ON_SUFFIX(2, "cy", "\xd1\x99");  // &ljcy;
				CHAR('l')
					ON_LEAF_NOBR(2, "\xe2\x89\xaa");  // &ll;
					CHAR_AT(2)
						CHAR('a') ON_SUFFIX(3, "rr", "\xe2\x87\x87");  // &llarr;
						CHAR('c') ON_SUFFIX(3, "orner", "\xe2\x8c\x9e");  // &llcorner;
						CHAR('h') ON_SUFFIX(3, "ard", "\xe2\xa5\xab");  // &llhard;
						CHAR('t') ON_SUFFIX(3, "ri", "\xe2\x97\xba");  // &lltri;
					CHAR_END()
				CHAR('m')
					CHAR_AT(2)
						CHAR('i') ON_SUFFIX(3, "dot", "\xc5\x80");  // &lmidot;
						CHAR('o')
							CHAR_AT(3)
								CHAR('u')
									CHAR_AT(4)
										CHAR('s')
											CHAR_AT(5)
												CHAR('t')
													ON_LEAF_NOBR(6, "\xe2\x8e\xb0");  // &lmoust;
													ON_SUFFIX(6, "ache", "\xe2\x8e\xb0");  // &lmoustache;
											CHAR_END()
									CHAR_END()
							CHAR_END()
					CHAR_END()
				CHAR('n')
					CHAR_AT(2)
						CHAR('E') ON_LEAF(3, "\xe2\x89\xa8");  // &lnE;
						CHAR('a')
							CHAR_AT(3)
								CHAR('p')
									ON_LEAF_NOBR(4, "\xe2\xaa\x89");  // &lnap;
									ON_SUFFIX(4, "prox", "\xe2\xaa\x89");  // &lnapprox;
							CHAR_END()
						CHAR('e')
							ON_LEAF_NOBR(3, "\xe2\xaa\x87");  // &lne;
							CHAR_AT(3)
								CHAR('q')
									ON_LEAF_NOBR(4, "\xe2\xaa\x87");  // &lneq;
									ON_SUFFIX(4, "q", "\xe2\x89\xa8");  // &lneqq;
							CHAR_END()
						CHAR('s') ON_SUFFIX(3, "im", "\xe2\x8b\xa6");  // &lnsim;
					CHAR_END()
				CHAR('o')
					CHAR_AT(2)
						CHAR('a')
							CHAR_AT(3)
								CHAR('n') ON_SUFFIX(4, "g", "\xe2\x9f\xac");  // &loang;
								CHAR('r') ON_SUFFIX(4, "r", "\xe2\x87\xbd");  // &loarr;
							CHAR_END()
						CHAR('b') ON_SUFFIX(3, "rk", "\xe2\x9f\xa6");  // &lobrk;
						CHAR('n')
							CHAR_AT(3)
								CHAR('g')
									CHAR_AT(4)
										CHAR('l')
											CHAR_AT(5)
												CHAR('e')
													CHAR_AT(6)
														CHAR('f')
															CHAR_AT(7)
																CHAR('t')
																	CHAR_AT(8)
																		CHAR('a') ON_SUFFIX(9, "rrow", "\xe2\x9f\xb5");  // &longleftarrow;
																		CHAR('r') ON_SUFFIX(9, "ightarrow", "\xe2\x9f\xb7");  // &longleftrightarrow;
																	CHAR_END()
															CHAR_END()
													CHAR_END()
											CHAR_END()
										CHAR('m') ON_SUFFIX(5, "apsto", "\xe2\x9f\xbc");  // &longmapsto;
										CHAR('r') ON_SUFFIX(5, "ightarrow", "\xe2\x9f\xb6");  // &longrightarrow;
									CHAR_END()
							CHAR_END()
						CHAR('o')
							CHAR_AT(3)
								CHAR('p')
									CHAR_AT(4)
										CHAR('a')
											CHAR_AT(5)
												CHAR('r')
													CHAR_AT(6)
														CHAR('r')
															CHAR_AT(7)
																CHAR('o')
																	CHAR_AT(8)
																		CHAR('w')
																			CHAR_AT(9)
																				CHAR('l') ON_SUFFIX(10, "eft", "\xe2\x86\xab");  // &looparrowleft;
																				CHAR('r') ON_SUFFIX(10, "ight", "\xe2\x86\xac");  // &looparrowright;
																			CHAR_END()
																	CHAR_END()
															CHAR_END()
													CHAR_END()
											CHAR_END()
									CHAR_END()
							CHAR_END()
						CHAR('p')
							CHAR_AT(3)
								CHAR('a') ON_SUFFIX(4, "r", "\xe2\xa6\x85");  // &lopar;
								CHAR('f') ON_LEAF(4, "\xf0\x9d\x95\x9d");  // &lopf;
								CHAR('l') ON_SUFFIX(4, "us", "\xe2\xa8\xad");  // &loplus;
							CHAR_END()
						CHAR('t') ON_SUFFIX(3, "imes", "\xe2\xa8\xb4");  // &lotimes;
						CHAR('w')
							CHAR_AT(3)
								CHAR('a') ON_SUFFIX(4, "st", "\xe2\x88\x97");  // &lowast;
								CHAR('b') ON_SUFFIX(4, "ar", "_");  // &lowbar;
							CHAR_END()
						CHAR('z')
							ON_LEAF_NOBR(3, "\xe2\x97\x8a");  // &loz;
							CHAR_AT(3)
								CHAR('e') ON_SUFFIX(4, "nge", "\xe2\x97\x8a");  // &lozenge;
								CHAR('f') ON_LEAF(4, "\xe2\xa7\xab");  // &lozf;
							CHAR_END()
					CHAR_END()
				CHAR('p')
					CHAR_AT(2)
						CHAR('a')
							CHAR_AT(3)
								CHAR('r')
									ON_LEAF_NOBR(4, "(");  // &lpar;
									ON_SUFFIX(4, "lt", "\xe2\xa6\x93");  // &lparlt;
							CHAR_END()
					CHAR_END()
				CHAR('r')
					CHAR_AT(2)
						CHAR('a') ON_SUFFIX(3, "rr", "\xe2\x87\x86");  // &lrarr;
						CHAR('c') ON_SUFFIX(3, "orner", "\xe2\x8c\x9f");  // &lrcorner;
						CHAR('h')
							CHAR_AT(3)
								CHAR('a')
									CHAR_AT(4)
										CHAR('r')
											ON_LEAF_NOBR(5, "\xe2\x87\x8b");  // &lrhar;
											ON_SUFFIX(5, "d", "\xe2\xa5\xad");  // &lrhard;
									CHAR_END()
							CHAR_END()
						CHAR('m') ON_LEAF(3, "\xe2\x80\x8e");  // &lrm;
						CHAR('t') ON_SUFFIX(3, "ri", "\xe2\x8a\xbf");  // &lrtri;
					CHAR_END()
				CHAR('s')
					CHAR_AT(2)
						CHAR('a') ON_SUFFIX(3, "quo", "\xe2\x80\xb9");  // &lsaquo;
						CHAR('c') ON_SUFFIX(3, "r", "\xf0\x9d\x93\x81");  // &lscr;
						CHAR('h') ON_LEAF(3, "\xe2\x86\xb0");  // &lsh;
						CHAR('i')
							CHAR_AT(3)
								CHAR('m')
									ON_LEAF_NOBR(4, "\xe2\x89\xb2");  // &lsim;
									CHAR_AT(4)
										CHAR('e') ON_LEAF(5, "\xe2\xaa\x8d");  // &lsime;
										CHAR('g') ON_LEAF(5, "\xe2\xaa\x8f");  // &lsimg;
									CHAR_END()
							CHAR_END()
						CHAR('q')
							CHAR_AT(3)
								CHAR('b') ON_LEAF(4, "[");  // &lsqb;
								CHAR('u')
									CHAR_AT(4)
										CHAR('o')
											ON_LEAF_NOBR(5, "\xe2\x80\x98");  // &lsquo;
											ON_SUFFIX(5, "r", "\xe2\x80\x9a");  // &lsquor;
									CHAR_END()
							CHAR_END()
						CHAR('t') ON_SUFFIX(3, "rok", "\xc5\x82");  // &lstrok;
					CHAR_END()
				CHAR('t')
					ON_LEAF_NOBR(2, "<");  // &lt;
					CHAR_AT(2)
						CHAR('c')
							CHAR_AT(3)
								CHAR('c') ON_LEAF(4, "\xe2\xaa\xa6");  // &ltcc;
								CHAR('i') ON_SUFFIX(4, "r", "\xe2\xa9\xb9");  // &ltcir;
							CHAR_END()
						CHAR('d') ON_SUFFIX(3, "ot", "\xe2\x8b\x96");  // &ltdot;
						CHAR('h') ON_SUFFIX(3, "ree", "\xe2\x8b\x8b");  // &lthree;
						CHAR('i') ON_SUFFIX(3, "mes", "\xe2\x8b\x89");  // &ltimes;
						CHAR('l') ON_SUFFIX(3, "arr", "\xe2\xa5\xb6");  // &ltlarr;
						CHAR('q') ON_SUFFIX(3, "uest", "\xe2\xa9\xbb");  // &ltquest;
						CHAR('r')
							CHAR_AT(3)
								CHAR('P') ON_SUFFIX(4, "ar", "\xe2\xa6\x96");  // &ltrPar;
								CHAR('i')
									ON_LEAF_NOBR(4, "\xe2\x97\x83");  // &ltri;
									CHAR_AT(4)
										CHAR('e') ON_LEAF(5, "\xe2\x8a\xb4");  // &ltrie;
										CHAR('f') ON_LEAF(5, "\xe2\x97\x82");  // &ltrif;
									CHAR_END()
							CHAR_END()
					CHAR_END()
				CHAR('u')
					CHAR_AT(2)
						CHAR('r')
							CHAR_AT(3)
								CHAR('d') ON_SUFFIX(4, "shar", "\xe2\xa5\x8a");  // &lurdshar;
								CHAR('u') ON_SUFFIX(4, "har", "\xe2\xa5\xa6");  // &luruhar;
							CHAR_END()
					CHAR_END()
				CHAR('v')
					CHAR_AT(2)
						CHAR('e') ON_SUFFIX(3, "rtneqq", "\xe2\x89\xa8\xef\xb8\x80");  // &lvertneqq;
						CHAR('n') ON_SUFFIX(3, "E", "\xe2\x89\xa8\xef\xb8\x80");  // &lvnE;
					CHAR_END()
			CHAR_END()
		CHAR('m')
			CHAR_AT(1)
				CHAR('D') ON_SUFFIX(2, "Dot", "\xe2\x88\xba");  // &mDDot;
				CHAR('a')
					CHAR_AT(2)
						CHAR('c') ON_SUFFIX(3, "r", "\xc2\xaf");  // &macr;
						CHAR('l')
							CHAR_AT(3)
								CHAR('e') ON_LEAF(4, "\xe2\x99\x82");  // &male;
								CHAR('t')
									ON_LEAF_NOBR(4, "\xe2\x9c\xa0");  // &malt;
									ON_SUFFIX(4, "ese", "\xe2\x9c\xa0");  // &maltese;
							CHAR_END()
						CHAR('p')
							ON_LEAF_NOBR(3, "\xe2\x86\xa6");  // &map;
							CHAR_AT(3)
								CHAR('s')
									CHAR_AT(4)
										CHAR('t')
											CHAR_AT(5)
												CHAR('o')
													ON_LEAF_NOBR(6, "\xe2\x86\xa6");  // &mapsto;
													CHAR_AT(6)
														CHAR('d') ON_SUFFIX(7, "own", "\xe2\x86\xa7");  // &mapstodown;
														CHAR('l') ON_SUFFIX(7, "eft", "\xe2\x86\xa4");  // &mapstoleft;
														CHAR('u') ON_SUFFIX(7, "p", "\xe2\x86\xa5");  // &mapstoup;
													CHAR_END()
											CHAR_END()
									CHAR_END()
							CHAR_END()
						CHAR('r') ON_SUFFIX(3, "ker", "\xe2\x96\xae");  // &marker;
					CHAR_END()
				CHAR('c')
					CHAR_AT(2)
						CHAR('o') ON_SUFFIX(3, "mma", "\xe2\xa8\xa9");  // &mcomma;
						CHAR('y') ON_LEAF(3, "\xd0\xbc");  // &mcy;
					CHAR_END()
				CHAR('d') ON_SUFFIX(2, "ash", "\xe2\x80\x94");  // &mdash;
				CHAR('e') ON_SUFFIX(2, "asuredangle", "\xe2\x88\xa1");  // &measuredangle;
				CHAR('f') ON_SUFFIX(2, "r", "\xf0\x9d\x94\xaa");  // &mfr;
				CHAR('h') ON_SUFFIX(2, "o", "\xe2\x84\xa7");  // &mho;
				CHAR('i')
					CHAR_AT(2)
						CHAR('c') ON_SUFFIX(3, "ro", "\xc2\xb5");  // &micro;
						CHAR('d')
							ON_LEAF_NOBR(3, "\xe2\x88\xa3");  // &mid;
							CHAR_AT(3)
								CHAR('a') ON_SUFFIX(4, "st", "*");  // &midast;
								CHAR('c') ON_SUFFIX(4, "ir", "\xe2\xab\xb0");  // &midcir;
								CHAR('d') ON_SUFFIX(4, "ot", "\xc2\xb7");  // &middot;
							CHAR_END()
						CHAR('n')
							CHAR_AT(3)
								CHAR('u')
									CHAR_AT(4)
										CHAR('s')
											ON_LEAF_NOBR(5, "\xe2\x88\x92");  // &minus;
											CHAR_AT(5)
												CHAR('b') ON_LEAF(6, "\xe2\x8a\x9f");  // &minusb;
												CHAR('d')
													ON_LEAF_NOBR(6, "\xe2\x88\xb8");  // &minusd;
													ON_SUFFIX(6, "u", "\xe2\xa8\xaa");  // &minusdu;
											CHAR_END()
									CHAR_END()
							CHAR_END()
					CHAR_END()
				CHAR('l')
					CHAR_AT(2)
						CHAR('c') ON_SUFFIX(3, "p", "\xe2\xab\x9b");  // &mlcp;
						CHAR('d') ON_SUFFIX(3, "r", "\xe2\x80\xa6");  // &mldr;
					CHAR_END()
				CHAR('n') ON_SUFFIX(2, "plus", "\xe2\x88\x93");  // &mnplus;
				CHAR('o')
					CHAR_AT(2)
						CHAR('d') ON_SUFFIX(3, "els", "\xe2\x8a\xa7");  // &models;
						CHAR('p') ON_SUFFIX(3, "f", "\xf0\x9d\x95\x9e");  // &mopf;
					CHAR_END()
				CHAR('p') ON_LEAF(2, "\xe2\x88\x93");  // &mp;
				CHAR('s')
					CHAR_AT(2)
						CHAR('c') ON_SUFFIX(3, "r", "\xf0\x9d\x93\x82");  // &mscr;
						CHAR('t') ON_SUFFIX(3, "pos", "\xe2\x88\xbe");  // &mstpos;
					CHAR_END()
				CHAR('u')
					ON_LEAF_NOBR(2, "\xce\xbc");  // &mu;
					CHAR_AT(2)
						CHAR('l') ON_SUFFIX(3, "timap", "\xe2\x8a\xb8");  // &multimap;
						CHAR('m') ON_SUFFIX(3, "ap", "\xe2\x8a\xb8");  // &mumap;
					CHAR_END()
			CHAR_END()
		CHAR('n')
			CHAR_AT(1)
				CHAR('G')
					CHAR_AT(2)
						CHAR('g') ON_LEAF(3, "\xe2\x8b\x99\xcc\xb8");  // &nGg;
						CHAR('t')
							ON_LEAF_NOBR(3, "\xe2\x89\xab\xe2\x83\x92");  // &nGt;
							ON_SUFFIX(3, "v", "\xe2\x89\xab\xcc\xb8");  // &nGtv;
					CHAR_END()
				CHAR('L')
					CHAR_AT(2)
						CHAR('e')
							CHAR_AT(3)
								CHAR('f')
									CHAR_AT(4)
										CHAR('t')
											CHAR_AT(5)
												CHAR('a') ON_SUFFIX(6, "rrow", "\xe2\x87\x8d");  // &nLeftarrow;
												CHAR('r') ON_SUFFIX(6, "ightarrow", "\xe2\x87\x8e");  // &nLeftrightarrow;
											CHAR_END()
									CHAR_END()
							CHAR_END()
						CHAR('l') ON_LEAF(3, "\xe2\x8b\x98\xcc\xb8");  // &nLl;
						CHAR('t')
							ON_LEAF_NOBR(3, "\xe2\x89\xaa\xe2\x83\x92");  // &nLt;
							ON_SUFFIX(3, "v", "\xe2\x89\xaa\xcc\xb8");  // &nLtv;
					CHAR_END()
				CHAR('R') ON_SUFFIX(2, "ightarrow", "\xe2\x87\x8f");  // &nRightarrow;
				CHAR('V')
					CHAR_AT(2)
						CHAR('D') ON_SUFFIX(3, "ash", "\xe2\x8a\xaf");  // &nVDash;
						CHAR('d') ON_SUFFIX(3, "ash", "\xe2\x8a\xae");  // &nVdash;
					CHAR_END()
				CHAR('a')
					CHAR_AT(2)
						CHAR('b') ON_SUFFIX(3, "la", "\xe2\x88\x87");  // &nabla;
						CHAR('c') ON_SUFFIX(3, "ute", "\xc5\x84");  // &nacute;
						CHAR('n') ON_SUFFIX(3, "g", "\xe2\x88\xa0\xe2\x83\x92");  // &nang;
						CHAR('p')
							ON_LEAF_NOBR(3, "\xe2\x89\x89");  // &nap;
							CHAR_AT(3)
								CHAR('E') ON_LEAF(4, "\xe2\xa9\xb0\xcc\xb8");  // &napE;
								CHAR('i') ON_SUFFIX(4, "d", "\xe2\x89\x8b\xcc\xb8");  // &napid;
								CHAR('o') ON_SUFFIX(4, "s", "\xc5\x89");  // &napos;
								CHAR('p') ON_SUFFIX(4, "rox", "\xe2\x89\x89");  // &napprox;
							CHAR_END()
						CHAR('t')
							CHAR_AT(3)
								CHAR('u')
									CHAR_AT(4)
										CHAR('r')
											ON_LEAF_NOBR(5, "\xe2\x99\xae");  // &natur;
											CHAR_AT(5)
												CHAR('a')
													CHAR_AT(6)
														CHAR('l')
															ON_LEAF_NOBR(7, "\xe2\x99\xae");  // &natural;
															ON_SUFFIX(7, "s", "\xe2\x84\x95");  // &naturals;
													CHAR_END()
											CHAR_END()
									CHAR_END()
							CHAR_END()
					CHAR_END()
				CHAR('b')
					CHAR_AT(2)
						CHAR('s') ON_SUFFIX(3, "p", "\xc2\xa0");  // &nbsp;
						CHAR('u')
							CHAR_AT(3)
								CHAR('m')
									CHAR_AT(4)
										CHAR('p')
											ON_LEAF_NOBR(5, "\xe2\x89\x8e\xcc\xb8");  // &nbump;
											ON_SUFFIX(5, "e", "\xe2\x89\x8f\xcc\xb8");  // &nbumpe;
									CHAR_END()
							CHAR_END()
					CHAR_END()
				CHAR('c')
					CHAR_AT(2)
						CHAR('a')
							CHAR_AT(3)
								CHAR('p') ON_LEAF(4, "\xe2\xa9\x83");  // &ncap;
								CHAR('r') ON_SUFFIX(4, "on", "\xc5\x88");  // &ncaron;
							CHAR_END()
						CHAR('e') ON_SUFFIX(3, "dil", "\xc5\x86");  // &ncedil;
						CHAR('o')
							CHAR_AT(3)
								CHAR('n')
									CHAR_AT(4)
										CHAR('g')
											ON_LEAF_NOBR(5, "\xe2\x89\x87");  // &ncong;
											ON_SUFFIX(5, "dot", "\xe2\xa9\xad\xcc\xb8");  // &ncongdot;
									CHAR_END()
							CHAR_END()
						CHAR('u') ON_SUFFIX(3, "p", "\xe2\xa9\x82");  // &ncup;
						CHAR('y') ON_LEAF(3, "\xd0\xbd");  // &ncy;
					CHAR_END()
				CHAR('d') ON_SUFFIX(2, "ash", "\xe2\x80\x93");  // &ndash;
				CHAR('e')
					ON_LEAF_NOBR(2, "\xe2\x89\xa0");  // &ne;
					CHAR_AT(2)
						CHAR('A') ON_SUFFIX(3, "rr", "\xe2\x87\x97");  // &neArr;
						CHAR('a')
							CHAR_AT(3)
								CHAR('r')
									CHAR_AT(4)
										CHAR('h') ON_SUFFIX(5, "k", "\xe2\xa4\xa4");  // &nearhk;
										CHAR('r')
											ON_LEAF_NOBR(5, "\xe2\x86\x97");  // &nearr;
											ON_SUFFIX(5, "ow", "\xe2\x86\x97");  // &nearrow;
									CHAR_END()
							CHAR_END()
						CHAR('d') ON_SUFFIX(3, "ot", "\xe2\x89\x90\xcc\xb8");  // &nedot;
						CHAR('q') ON_SUFFIX(3, "uiv", "\xe2\x89\xa2");  // &nequiv;
						CHAR('s')
							CHAR_AT(3)
								CHAR('e') ON_SUFFIX(4, "ar", "\xe2\xa4\xa8");  // &nesear;
								CHAR('i') ON_SUFFIX(4, "m", "\xe2\x89\x82\xcc\xb8");  // &nesim;
							CHAR_END()
						CHAR('x')
							CHAR_AT(3)
								CHAR('i')
									CHAR_AT(4)
										CHAR('s')
											CHAR_AT(5)
												CHAR('t')
													ON_LEAF_NOBR(6, "\xe2\x88\x84");  // &nexist;
													ON_SUFFIX(6, "s", "\xe2\x88\x84");  // &nexists;
											CHAR_END()
									CHAR_END()
							CHAR_END()
					CHAR_END()
				CHAR('f') ON_SUFFIX(2, "r", "\xf0\x9d\x94\xab");  // &nfr;
				CHAR('g')
					CHAR_AT(2)
						CHAR('E') ON_LEAF(3, "\xe2\x89\xa7\xcc\xb8");  // &ngE;
						CHAR('e')
							ON_LEAF_NOBR(3, "\xe2\x89\xb1");  // &nge;
							CHAR_AT(3)
								CHAR('q')
									ON_LEAF_NOBR(4, "\xe2\x89\xb1");  // &ngeq;
									CHAR_AT(4)
										CHAR('q') ON_LEAF(5, "\xe2\x89\xa7\xcc\xb8");  // &ngeqq;
										CHAR('s') ON_SUFFIX(5, "lant", "\xe2\xa9\xbe\xcc\xb8");  // &ngeqslant;
									CHAR_END()
								CHAR('s') ON_LEAF(4, "\xe2\xa9\xbe\xcc\xb8");  // &nges;
							CHAR_END()
						CHAR('s') ON_SUFFIX(3, "im", "\xe2\x89\xb5");  // &ngsim;
						CHAR('t')
							ON_LEAF_NOBR(3, "\xe2\x89\xaf");  // &ngt;
							ON_SUFFIX(3, "r", "\xe2\x89\xaf");  // &ngtr;
					CHAR_END()
				CHAR('h')
					CHAR_AT(2)
						CHAR('A') ON_SUFFIX(3, "rr", "\xe2\x87\x8e");  // &nhArr;
						CHAR('a') ON_SUFFIX(3, "rr", "\xe2\x86\xae");  // &nharr;
						CHAR('p') ON_SUFFIX(3, "ar", "\xe2\xab\xb2");  // &nhpar;
					CHAR_END()
				CHAR('i')
					ON_LEAF_NOBR(2, "\xe2\x88\x8b");  // &ni;
					CHAR_AT(2)
						CHAR('s')
							ON_LEAF_NOBR(3, "\xe2\x8b\xbc");  // &nis;
							ON_SUFFIX(3, "d", "\xe2\x8b\xba");  // &nisd;
						CHAR('v') ON_LEAF(3, "\xe2\x88\x8b");  // &niv;
					CHAR_END()
				CHAR('j') ON_SUFFIX(2, "cy", "\xd1\x9a");  // &njcy;
				CHAR('l')
					CHAR_AT(2)
						CHAR('A') ON_SUFFIX(3, "rr", "\xe2\x87\x8d");  // &nlArr;
						CHAR('E') ON_LEAF(3, "\xe2\x89\xa6\xcc\xb8");  // &nlE;
						CHAR('a') ON_SUFFIX(3, "rr", "\xe2\x86\x9a");  // &nlarr;
						CHAR('d') ON_SUFFIX(3, "r", "\xe2\x80\xa5");  // &nldr;
						CHAR('e')
							ON_LEAF_NOBR(3, "\xe2\x89\xb0");  // &nle;
							CHAR_AT(3)
								CHAR('f')
									CHAR_AT(4)
										CHAR('t')
											CHAR_AT(5)
												CHAR('a') ON_SUFFIX(6, "rrow", "\xe2\x86\x9a");  // &nleftarrow;
												CHAR('r') ON_SUFFIX(6, "ightarrow", "\xe2\x86\xae");  // &nleftrightarrow;
											CHAR_END()
									CHAR_END()
								CHAR('q')
									ON_LEAF_NOBR(4, "\xe2\x89\xb0");  // &nleq;
									CHAR_AT(4)
										CHAR('q') ON_LEAF(5, "\xe2\x89\xa6\xcc\xb8");  // &nleqq;
										CHAR('s') ON_SUFFIX(5, "lant", "\xe2\xa9\xbd\xcc\xb8");  // &nleqslant;
									CHAR_END()
								CHAR('s')
									ON_LEAF_NOBR(4, "\xe2\xa9\xbd\xcc\xb8");  // &nles;
									ON_SUFFIX(4, "s", "\xe2\x89\xae");  // &nless;
							CHAR_END()
						CHAR('s') ON_SUFFIX(3, "im", "\xe2\x89\xb4");  // &nlsim;
						CHAR('t')
							ON_LEAF_NOBR(3, "\xe2\x89\xae");  // &nlt;
							CHAR_AT(3)
								CHAR('r')
									CHAR_AT(4)
										CHAR('i')
											ON_LEAF_NOBR(5, "\xe2\x8b\xaa");  // &nltri;
											ON_SUFFIX(5, "e", "\xe2\x8b\xac");  // &nltrie;
									CHAR_END()
							CHAR_END()
					CHAR_END()
				CHAR('m') ON_SUFFIX(2, "id", "\xe2\x88\xa4");  // &nmid;
				CHAR('o')
					CHAR_AT(2)
						CHAR('p') ON_SUFFIX(3, "f", "\xf0\x9d\x95\x9f");  // &nopf;
						CHAR('t')
							ON_LEAF_NOBR(3, "\xc2\xac");  // &not;
							CHAR_AT(3)
								CHAR('i')
									CHAR_AT(4)
										CHAR('n')
											ON_LEAF_NOBR(5, "\xe2\x88\x89");  // &notin;
											CHAR_AT(5)
												CHAR('E') ON_LEAF(6, "\xe2\x8b\xb9\xcc\xb8");  // &notinE;
												CHAR('d') ON_SUFFIX(6, "ot", "\xe2\x8b\xb5\xcc\xb8");  // &notindot;
												CHAR('v')
													CHAR_AT(6)
														CHAR('a') ON_LEAF(7, "\xe2\x88\x89");  // &notinva;
														CHAR('b') ON_LEAF(7, "\xe2\x8b\xb7");  // &notinvb;
														CHAR('c') ON_LEAF(7, "\xe2\x8b\xb6");  // &notinvc;
													CHAR_END()
											CHAR_END()
									CHAR_END()
								CHAR('n')
									CHAR_AT(4)
										CHAR('i')
											ON_LEAF_NOBR(5, "\xe2\x88\x8c");  // &notni;
											CHAR_AT(5)
												CHAR('v')
													CHAR_AT(6)
														CHAR('a') ON_LEAF(7, "\xe2\x88\x8c");  // &notniva;
														CHAR('b') ON_LEAF(7, "\xe2\x8b\xbe");  // &notnivb;
														CHAR('c') ON_LEAF(7, "\xe2\x8b\xbd");  // &notnivc;
													CHAR_END()
											CHAR_END()
									CHAR_END()
							CHAR_END()
					CHAR_END()
				CHAR('p')
					CHAR_AT(2)
						CHAR('a')
							CHAR_AT(3)
								CHAR('r')
									ON_LEAF_NOBR(4, "\xe2\x88\xa6");  // &npar;
									CHAR_AT(4)
										CHAR('a') ON_SUFFIX(5, "llel", "\xe2\x88\xa6");  // &nparallel;
										CHAR('s') ON_SUFFIX(5, "l", "\xe2\xab\xbd\xe2\x83\xa5");  // &nparsl;
										CHAR('t') ON_LEAF(5, "\xe2\x88\x82\xcc\xb8");  // &npart;
									CHAR_END()
							CHAR_END()
						CHAR('o') ON_SUFFIX(3, "lint", "\xe2\xa8\x94");  // &npolint;
						CHAR('r')
							ON_LEAF_NOBR(3, "\xe2\x8a\x80");  // &npr;
							CHAR_AT(3)
								CHAR('c') ON_SUFFIX(4, "ue", "\xe2\x8b\xa0");  // &nprcue;
								CHAR('e')
									ON_LEAF_NOBR(4, "\xe2\xaa\xaf\xcc\xb8");  // &npre;
									CHAR_AT(4)
										CHAR('c')
											ON_LEAF_NOBR(5, "\xe2\x8a\x80");  // &nprec;
											ON_SUFFIX(5, "eq", "\xe2\xaa\xaf\xcc\xb8");  // &npreceq;
									CHAR_END()
							CHAR_END()
					CHAR_END()
				CHAR('r')
					CHAR_AT(2)
						CHAR('A') ON_SUFFIX(3, "rr", "\xe2\x87\x8f");  // &nrArr;
						CHAR('a')
							CHAR_AT(3)
								CHAR('r')
									CHAR_AT(4)
										CHAR('r')
											ON_LEAF_NOBR(5, "\xe2\x86\x9b");  // &nrarr;
											CHAR_AT(5)
												CHAR('c') ON_LEAF(6, "\xe2\xa4\xb3\xcc\xb8");  // &nrarrc;
												CHAR('w') ON_LEAF(6, "\xe2\x86\x9d\xcc\xb8");  // &nrarrw;
											CHAR_END()
									CHAR_END()
							CHAR_END()
						CHAR('i') ON_SUFFIX(3, "ghtarrow", "\xe2\x86\x9b");  // &nrightarrow;
						CHAR('t')
							CHAR_AT(3)
								CHAR('r')
									CHAR_AT(4)
										CHAR('i')
											ON_LEAF_NOBR(5, "\xe2\x8b\xab");  // &nrtri;
											ON_SUFFIX(5, "e", "\xe2\x8b\xad");  // &nrtrie;
									CHAR_END()
							CHAR_END()
					CHAR_END()
				CHAR('s')
					CHAR_AT(2)
						CHAR('c')
							ON_LEAF_NOBR(3, "\xe2\x8a\x81");  // &nsc;
							CHAR_AT(3)
								CHAR('c') ON_SUFFIX(4, "ue", "\xe2\x8b\xa1");  // &nsccue;
								CHAR('e') ON_LEAF(4, "\xe2\xaa\xb0\xcc\xb8");  // &nsce;
								CHAR('r') ON_LEAF(4, "\xf0\x9d\x93\x83");  // &nscr;
							CHAR_END()
						CHAR('h')
							CHAR_AT(3)
								CHAR('o')
									CHAR_AT(4)
										CHAR('r')
											CHAR_AT(5)
												CHAR('t')
													CHAR_AT(6)
														CHAR('m') ON_SUFFIX(7, "id", "\xe2\x88\xa4");  // &nshortmid;
														CHAR('p') ON_SUFFIX(7, "arallel", "\xe2\x88\xa6");  // &nshortparallel;
													CHAR_END()
											CHAR_END()
									CHAR_END()
							CHAR_END()
						CHAR('i')
							CHAR_AT(3)
								CHAR('m')
									ON_LEAF_NOBR(4, "\xe2\x89\x81");  // &nsim;
									CHAR_AT(4)
										CHAR('e')
											ON_LEAF_NOBR(5, "\xe2\x89\x84");  // &nsime;
											ON_SUFFIX(5, "q", "\xe2\x89\x84");  // &nsimeq;
									CHAR_END()
							CHAR_END()
						CHAR('m') ON_SUFFIX(3, "id", "\xe2\x88\xa4");  // &nsmid;
						CHAR('p') ON_SUFFIX(3, "ar", "\xe2\x88\xa6");  // &nspar;
						CHAR('q')
							CHAR_AT(3)
								CHAR('s')
									CHAR_AT(4)
										CHAR('u')
											CHAR_AT(5)
												CHAR('b') ON_SUFFIX(6, "e", "\xe2\x8b\xa2");  // &nsqsube;
												CHAR('p') ON_SUFFIX(6, "e", "\xe2\x8b\xa3");  // &nsqsupe;
											CHAR_END()
									CHAR_END()
							CHAR_END()
						CHAR('u')
							CHAR_AT(3)
								CHAR('b')
									ON_LEAF_NOBR(4, "\xe2\x8a\x84");  // &nsub;
									CHAR_AT(4)
										CHAR('E') ON_LEAF(5, "\xe2\xab\x85\xcc\xb8");  // &nsubE;
										CHAR('e') ON_LEAF(5, "\xe2\x8a\x88");  // &nsube;
										CHAR('s')
											CHAR_AT(5)
												CHAR('e')
													CHAR_AT(6)
														CHAR('t')
															ON_LEAF_NOBR(7, "\xe2\x8a\x82\xe2\x83\x92");  // &nsubset;
															CHAR_AT(7)
																CHAR('e')
																	CHAR_AT(8)
																		CHAR('q')
																			ON_LEAF_NOBR(9, "\xe2\x8a\x88");  // &nsubseteq;
																			ON_SUFFIX(9, "q", "\xe2\xab\x85\xcc\xb8");  // &nsubseteqq;
																	CHAR_END()
															CHAR_END()
													CHAR_END()
											CHAR_END()
									CHAR_END()
								CHAR('c')
									CHAR_AT(4)
										CHAR('c')
											ON_LEAF_NOBR(5, "\xe2\x8a\x81");  // &nsucc;
											ON_SUFFIX(5, "eq", "\xe2\xaa\xb0\xcc\xb8");  // &nsucceq;
									CHAR_END()
								CHAR('p')
									ON_LEAF_NOBR(4, "\xe2\x8a\x85");  // &nsup;
									CHAR_AT(4)
										CHAR('E') ON_LEAF(5, "\xe2\xab\x86\xcc\xb8");  // &nsupE;
										CHAR('e') ON_LEAF(5, "\xe2\x8a\x89");  // &nsupe;
										CHAR('s')
											CHAR_AT(5)
												CHAR('e')
													CHAR_AT(6)
														CHAR('t')
															ON_LEAF_NOBR(7, "\xe2\x8a\x83\xe2\x83\x92");  // &nsupset;
															CHAR_AT(7)
																CHAR('e')
																	CHAR_AT(8)
																		CHAR('q')
																			ON_LEAF_NOBR(9, "\xe2\x8a\x89");  // &nsupseteq;
																			ON_SUFFIX(9, "q", "\xe2\xab\x86\xcc\xb8");  // &nsupseteqq;
																	CHAR_END()
															CHAR_END()
													CHAR_END()
											CHAR_END()
									CHAR_END()
							CHAR_END()
					CHAR_END()
				CHAR('t')
					CHAR_AT(2)
						CHAR('g') ON_SUFFIX(3, "l", "\xe2\x89\xb9");  // &ntgl;
						CHAR('i') ON_SUFFIX(3, "lde", "\xc3\xb1");  // &ntilde;
						CHAR('l') ON_SUFFIX(3, "g", "\xe2\x89\xb8");  // &ntlg;
						CHAR('r')
							CHAR_AT(3)
								CHAR('i')
									CHAR_AT(4)
										CHAR('a')
											CHAR_AT(5)
												CHAR('n')
													CHAR_AT(6)
														CHAR('g')
															CHAR_AT(7)
																CHAR('l')
																	CHAR_AT(8)
																		CHAR('e')
																			CHAR_AT(9)
																				CHAR('l')
																					CHAR_AT(10)
																						CHAR('e')
																							CHAR_AT(11)
																								CHAR('f')
																									CHAR_AT(12)
																										CHAR('t')
																											ON_LEAF_NOBR(13, "\xe2\x8b\xaa");  // &ntriangleleft;
																											ON_SUFFIX(13, "eq", "\xe2\x8b\xac");  // &ntrianglelefteq;
																									CHAR_END()
																							CHAR_END()
																					CHAR_END()
																				CHAR('r')
																					CHAR_AT(10)
																						CHAR('i')
																							CHAR_AT(11)
																								CHAR('g')
																									CHAR_AT(12)
																										CHAR('h')
																											CHAR_AT(13)
																												CHAR('t')
																													ON_LEAF_NOBR(14, "\xe2\x8b\xab");  // &ntriangleright;
																													ON_SUFFIX(14, "eq", "\xe2\x8b\xad");  // &ntrianglerighteq;
																											CHAR_END()
																									CHAR_END()
																							CHAR_END()
																					CHAR_END()
																			CHAR_END()
																	CHAR_END()
															CHAR_END()
													CHAR_END()
											CHAR_END()
									CHAR_END()
							CHAR_END()
					CHAR_END()
				CHAR('u')
					ON_LEAF_NOBR(2, "\xce\xbd");  // &nu;
					CHAR_AT(2)
						CHAR('m')
							ON_LEAF_NOBR(3, "#");  // &num;
							CHAR_AT(3)
								CHAR('e') ON_SUFFIX(4, "ro", "\xe2\x84\x96");  // &numero;
								CHAR('s') ON_SUFFIX(4, "p", "\xe2\x80\x87");  // &numsp;
							CHAR_END()
					CHAR_END()
				CHAR('v')
					CHAR_AT(2)
						CHAR('D') ON_SUFFIX(3, "ash", "\xe2\x8a\xad");  // &nvDash;
						CHAR('H') ON_SUFFIX(3, "arr", "\xe2\xa4\x84");  // &nvHarr;
						CHAR('a') ON_SUFFIX(3, "p", "\xe2\x89\x8d\xe2\x83\x92");  // &nvap;
						CHAR('d') ON_SUFFIX(3, "ash", "\xe2\x8a\xac");  // &nvdash;
						CHAR('g')
							CHAR_AT(3)
								CHAR('e') ON_LEAF(4, "\xe2\x89\xa5\xe2\x83\x92");  // &nvge;
								CHAR('t') ON_LEAF(4, ">\xe2\x83\x92");  // &nvgt;
							CHAR_END()
						CHAR('i') ON_SUFFIX(3, "nfin", "\xe2\xa7\x9e");  // &nvinfin;
						CHAR('l')
							CHAR_AT(3)
								CHAR('A') ON_SUFFIX(4, "rr", "\xe2\xa4\x82");  // &nvlArr;
								CHAR('e') ON_LEAF(4, "\xe2\x89\xa4\xe2\x83\x92");  // &nvle;
								CHAR('t')
									ON_LEAF_NOBR(4, "<\xe2\x83\x92");  // &nvlt;
									ON_SUFFIX(4, "rie", "\xe2\x8a\xb4\xe2\x83\x92");  // &nvltrie;
							CHAR_END()
						CHAR('r')
							CHAR_AT(3)
								CHAR('A') ON_SUFFIX(4, "rr", "\xe2\xa4\x83");  // &nvrArr;
								CHAR('t') ON_SUFFIX(4, "rie", "\xe2\x8a\xb5\xe2\x83\x92");  // &nvrtrie;
							CHAR_END()
						CHAR('s') ON_SUFFIX(3, "im", "\xe2\x88\xbc\xe2\x83\x92");  // &nvsim;
					CHAR_END()
				CHAR('w')
					CHAR_AT(2)
						CHAR('A') ON_SUFFIX(3, "rr", "\xe2\x87\x96");  // &nwArr;
						CHAR('a')
							CHAR_AT(3)
								CHAR('r')
									CHAR_AT(4)
										CHAR('h') ON_SUFFIX(5, "k", "\xe2\xa4\xa3");  // &nwarhk;
										CHAR('r')
											ON_LEAF_NOBR(5, "\xe2\x86\x96");  // &nwarr;
											ON_SUFFIX(5, "ow", "\xe2\x86\x96");  // &nwarrow;
									CHAR_END()
							CHAR_END()
						CHAR('n') ON_SUFFIX(3, "ear", "\xe2\xa4\xa7");  // &nwnear;
					CHAR_END()
			CHAR_END()
		CHAR('o')
			CHAR_AT(1)
				CHAR('S') ON_LEAF(2, "\xe2\x93\x88");  // &oS;
				CHAR('a')
					CHAR_AT(2)
						CHAR('c') ON_SUFFIX(3, "ute", "\xc3\xb3");  // &oacute;
						CHAR('s') ON_SUFFIX(3, "t", "\xe2\x8a\x9b");  // &oast;
					CHAR_END()
				CHAR('c')
					CHAR_AT(2)
						CHAR('i')
							CHAR_AT(3)
								CHAR('r')
									ON_LEAF_NOBR(4, "\xe2\x8a\x9a");  // &ocir;
									ON_SUFFIX(4, "c", "\xc3\xb4");  // &ocirc;
							CHAR_END()
						CHAR('y') ON_LEAF(3, "\xd0\xbe");  // &ocy;
					CHAR_END()
				CHAR('d')
					CHAR_AT(2)
						CHAR('a') ON_SUFFIX(3, "sh", "\xe2\x8a\x9d");  // &odash;
						CHAR('b') ON_SUFFIX(3, "lac", "\xc5\x91");  // &odblac;
						CHAR('i') ON_SUFFIX(3, "v", "\xe2\xa8\xb8");  // &odiv;
						CHAR('o') ON_SUFFIX(3, "t", "\xe2\x8a\x99");  // &odot;
						CHAR('s') ON_SUFFIX(3, "old", "\xe2\xa6\xbc");  // &odsold;
					CHAR_END()
				CHAR('e') ON_SUFFIX(2, "lig", "\xc5\x93");  // &oelig;
				CHAR('f')
					CHAR_AT(2)
						CHAR('c') ON_SUFFIX(3, "ir", "\xe2\xa6\xbf");  // &ofcir;
						CHAR('r') ON_LEAF(3, "\xf0\x9d\x94\xac");  // &ofr;
					CHAR_END()
				CHAR('g')
					CHAR_AT(2)
						CHAR('o') ON_SUFFIX(3, "n", "\xcb\x9b");  // &ogon;
						CHAR('r') ON_SUFFIX(3, "ave", "\xc3\xb2");  // &ograve;
						CHAR('t') ON_LEAF(3, "\xe2\xa7\x81");  // &ogt;
					CHAR_END()
				CHAR('h')
					CHAR_AT(2)
						CHAR('b') ON_SUFFIX(3, "ar", "\xe2\xa6\xb5");  // &ohbar;
						CHAR('m') ON_LEAF(3, "\xce\xa9");  // &ohm;
					CHAR_END()
				CHAR('i') ON_SUFFIX(2, "nt", "\xe2\x88\xae");  // &oint;
				CHAR('l')
					CHAR_AT(2)
						CHAR('a') ON_SUFFIX(3, "rr", "\xe2\x86\xba");  // &olarr;
						CHAR('c')
							CHAR_AT(3)
								CHAR('i') ON_SUFFIX(4, "r", "\xe2\xa6\xbe");  // &olcir;
								CHAR('r') ON_SUFFIX(4, "oss", "\xe2\xa6\xbb");  // &olcross;
							CHAR_END()
						CHAR('i') ON_SUFFIX(3, "ne", "\xe2\x80\xbe");  // &oline;
						CHAR('t') ON_LEAF(3, "\xe2\xa7\x80");  // &olt;
					CHAR_END()
				CHAR('m')
					CHAR_AT(2)
						CHAR('a') ON_SUFFIX(3, "cr", "\xc5\x8d");  // &omacr;
						CHAR('e') ON_SUFFIX(3, "ga", "\xcf\x89");  // &omega;
						CHAR('i')
							CHAR_AT(3)
								CHAR('c') ON_SUFFIX(4, "ron", "\xce\xbf");  // &omicron;
								CHAR('d') ON_LEAF(4, "\xe2\xa6\xb6");  // &omid;
								CHAR('n') ON_SUFFIX(4, "us", "\xe2\x8a\x96");  // &ominus;
							CHAR_END()
					CHAR_END()
				CHAR('o') ON_SUFFIX(2, "pf", "\xf0\x9d\x95\xa0");  // &oopf;
				CHAR('p')
					CHAR_AT(2)
						CHAR('a') ON_SUFFIX(3, "r", "\xe2\xa6\xb7");  // &opar;
						CHAR('e') ON_SUFFIX(3, "rp", "\xe2\xa6\xb9");  // &operp;
						CHAR('l') ON_SUFFIX(3, "us", "\xe2\x8a\x95");  // &oplus;
					CHAR_END()
				CHAR('r')
					ON_LEAF_NOBR(2, "\xe2\x88\xa8");  // &or;
					CHAR_AT(2)
						CHAR('a') ON_SUFFIX(3, "rr", "\xe2\x86\xbb");  // &orarr;
						CHAR('d')
							ON_LEAF_NOBR(3, "\xe2\xa9\x9d");  // &ord;
							CHAR_AT(3)
								CHAR('e')
									CHAR_AT(4)
										CHAR('r')
											ON_LEAF_NOBR(5, "\xe2\x84\xb4");  // &order;
											ON_SUFFIX(5, "of", "\xe2\x84\xb4");  // &orderof;
									CHAR_END()
								CHAR('f') ON_LEAF(4, "\xc2\xaa");  // &ordf;
								CHAR('m') ON_LEAF(4, "\xc2\xba");  // &ordm;
							CHAR_END()
						CHAR('i') ON_SUFFIX(3, "gof", "\xe2\x8a\xb6");  // &origof;
						CHAR('o') ON_SUFFIX(3, "r", "\xe2\xa9\x96");  // &oror;
						CHAR('s') ON_SUFFIX(3, "lope", "\xe2\xa9\x97");  // &orslope;
						CHAR('v') ON_LEAF(3, "\xe2\xa9\x9b");  // &orv;
					CHAR_END()
				CHAR('s')
					CHAR_AT(2)
						CHAR('c') ON_SUFFIX(3, "r", "\xe2\x84\xb4");  // &oscr;
						CHAR('l') ON_SUFFIX(3, "ash", "\xc3\xb8");  // &oslash;
						CHAR('o') ON_SUFFIX(3, "l", "\xe2\x8a\x98");  // &osol;
					CHAR_END()
				CHAR('t')
					CHAR_AT(2)
						CHAR('i')
							CHAR_AT(3)
								CHAR('l') ON_SUFFIX(4, "de", "\xc3\xb5");  // &otilde;
								CHAR('m')
									CHAR_AT(4)
										CHAR('e')
											CHAR_AT(5)
												CHAR('s')
													ON_LEAF_NOBR(6, "\xe2\x8a\x97");  // &otimes;
													ON_SUFFIX(6, "as", "\xe2\xa8\xb6");  // &otimesas;
											CHAR_END()
									CHAR_END()
							CHAR_END()
					CHAR_END()
				CHAR('u') ON_SUFFIX(2, "ml", "\xc3\xb6");  // &ouml;
				CHAR('v') ON_SUFFIX(2, "bar", "\xe2\x8c\xbd");  // &ovbar;
			CHAR_END()
		CHAR('p')
			CHAR_AT(1)
				CHAR('a')
					CHAR_AT(2)
						CHAR('r')
							ON_LEAF_NOBR(3, "\xe2\x88\xa5");  // &par;
							CHAR_AT(3)
								CHAR('a')
									ON_LEAF_NOBR(4, "\xc2\xb6");  // &para;
									ON_SUFFIX(4, "llel", "\xe2\x88\xa5");  // &parallel;
								CHAR('s')
									CHAR_AT(4)
										CHAR('i') ON_SUFFIX(5, "m", "\xe2\xab\xb3");  // &parsim;
										CHAR('l') ON_LEAF(5, "\xe2\xab\xbd");  // &parsl;
									CHAR_END()
								CHAR('t') ON_LEAF(4, "\xe2\x88\x82");  // &part;
							CHAR_END()
					CHAR_END()
				CHAR('c') ON_SUFFIX(2, "y", "\xd0\xbf");  // &pcy;
				CHAR('e')
					CHAR_AT(2)
						CHAR('r')
							CHAR_AT(3)
								CHAR('c') ON_SUFFIX(4, "nt", "%");  // &percnt;
								CHAR('i') ON_SUFFIX(4, "od", ".");  // &period;
								CHAR('m') ON_SUFFIX(4, "il", "\xe2\x80\xb0");  // &permil;
								CHAR('p') ON_LEAF(4, "\xe2\x8a\xa5");  // &perp;
								CHAR('t') ON_SUFFIX(4, "enk", "\xe2\x80\xb1");  // &pertenk;
							CHAR_END()
					CHAR_END()
				CHAR('f') ON_SUFFIX(2, "r", "\xf0\x9d\x94\xad");  // &pfr;
				CHAR('h')
					CHAR_AT(2)
						CHAR('i')
							ON_LEAF_NOBR(3, "\xcf\x86");  // &phi;
							ON_SUFFIX(3, "v", "\xcf\x95");  // &phiv;
						CHAR('m') ON_SUFFIX(3, "mat", "\xe2\x84\xb3");  // &phmmat;
						CHAR('o') ON_SUFFIX(3, "ne", "\xe2\x98\x8e");  // &phone;
					CHAR_END()
				CHAR('i')
					ON_LEAF_NOBR(2, "\xcf\x80");  // &pi;
					CHAR_AT(2)
						CHAR('t') ON_SUFFIX(3, "chfork", "\xe2\x8b\x94");  // &pitchfork;
						CHAR('v') ON_LEAF(3, "\xcf\x96");  // &piv;
					CHAR_END()
				CHAR('l')
					CHAR_AT(2)
						CHAR('a')
							CHAR_AT(3)
								CHAR('n')
									CHAR_AT(4)
										CHAR('c')
											CHAR_AT(5)
												CHAR('k')
													ON_LEAF_NOBR(6, "\xe2\x84\x8f");  // &planck;
													ON_SUFFIX(6, "h", "\xe2\x84\x8e");  // &planckh;
											CHAR_END()
										CHAR('k') ON_SUFFIX(5, "v", "\xe2\x84\x8f");  // &plankv;
									CHAR_END()
							CHAR_END()
						CHAR('u')
							CHAR_AT(3)
								CHAR('s')
									ON_LEAF_NOBR(4, "+");  // &plus;
									CHAR_AT(4)
										CHAR('a') ON_SUFFIX(5, "cir", "\xe2\xa8\xa3");  // &plusacir;
										CHAR('b') ON_LEAF(5, "\xe2\x8a\x9e");  // &plusb;
										CHAR('c') ON_SUFFIX(5, "ir", "\xe2\xa8\xa2");  // &pluscir;
										CHAR('d')
											CHAR_AT(5)
												CHAR('o') ON_LEAF(6, "\xe2\x88\x94");  // &plusdo;
												CHAR('u') ON_LEAF(6, "\xe2\xa8\xa5");  // &plusdu;
											CHAR_END()
										CHAR('e') ON_LEAF(5, "\xe2\xa9\xb2");  // &pluse;
										CHAR('m') ON_SUFFIX(5, "n", "\xc2\xb1");  // &plusmn;
										CHAR('s') ON_SUFFIX(5, "im", "\xe2\xa8\xa6");  // &plussim;
										CHAR('t') ON_SUFFIX(5, "wo", "\xe2\xa8\xa7");  // &plustwo;
									CHAR_END()
							CHAR_END()
					CHAR_END()
				CHAR('m') ON_LEAF(2, "\xc2\xb1");  // &pm;
				CHAR('o')
					CHAR_AT(2)
						CHAR('i') ON_SUFFIX(3, "ntint", "\xe2\xa8\x95");  // &pointint;
						CHAR('p') ON_SUFFIX(3, "f", "\xf0\x9d\x95\xa1");  // &popf;
						CHAR('u') ON_SUFFIX(3, "nd", "\xc2\xa3");  // &pound;
					CHAR_END()
				CHAR('r')
					ON_LEAF_NOBR(2, "\xe2\x89\xba");  // &pr;
					CHAR_AT(2)
						CHAR('E') ON_LEAF(3, "\xe2\xaa\xb3");  // &prE;
						CHAR('a') ON_SUFFIX(3, "p", "\xe2\xaa\xb7");  // &prap;
						CHAR('c') ON_SUFFIX(3, "ue", "\xe2\x89\xbc");  // &prcue;
						CHAR('e')
							ON_LEAF_NOBR(3, "\xe2\xaa\xaf");  // &pre;
							CHAR_AT(3)
								CHAR('c')
									ON_LEAF_NOBR(4, "\xe2\x89\xba");  // &prec;
									CHAR_AT(4)
										CHAR('a') ON_SUFFIX(5, "pprox", "\xe2\xaa\xb7");  // &precapprox;
										CHAR('c') ON_SUFFIX(5, "urlyeq", "\xe2\x89\xbc");  // &preccurlyeq;
										CHAR('e') ON_SUFFIX(5, "q", "\xe2\xaa\xaf");  // &preceq;
										CHAR('n')
											CHAR_AT(5)
												CHAR('a') ON_SUFFIX(6, "pprox", "\xe2\xaa\xb9");  // &precnapprox;
												CHAR('e') ON_SUFFIX(6, "qq", "\xe2\xaa\xb5");  // &precneqq;
												CHAR('s') ON_SUFFIX(6, "im", "\xe2\x8b\xa8");  // &precnsim;
											CHAR_END()
										CHAR('s') ON_SUFFIX(5, "im", "\xe2\x89\xbe");  // &precsim;
									CHAR_END()
							CHAR_END()
						CHAR('i')
							CHAR_AT(3)
								CHAR('m')
									CHAR_AT(4)
										CHAR('e')
											ON_LEAF_NOBR(5, "\xe2\x80\xb2");  // &prime;
											ON_SUFFIX(5, "s", "\xe2\x84\x99");  // &primes;
									CHAR_END()
							CHAR_END()
						CHAR('n')
							CHAR_AT(3)
								CHAR('E') ON_LEAF(4, "\xe2\xaa\xb5");  // &prnE;
								CHAR('a') ON_SUFFIX(4, "p", "\xe2\xaa\xb9");  // &prnap;
								CHAR('s') ON_SUFFIX(4, "im", "\xe2\x8b\xa8");  // &prnsim;
							CHAR_END()
						CHAR('o')
							CHAR_AT(3)
								CHAR('d') ON_LEAF(4, "\xe2\x88\x8f");  // &prod;
								CHAR('f')
									CHAR_AT(4)
										CHAR('a') ON_SUFFIX(5, "lar", "\xe2\x8c\xae");  // &profalar;
										CHAR('l') ON_SUFFIX(5, "ine", "\xe2\x8c\x92");  // &profline;
										CHAR('s') ON_SUFFIX(5, "urf", "\xe2\x8c\x93");  // &profsurf;
									CHAR_END()
								CHAR('p')
									ON_LEAF_NOBR(4, "\xe2\x88\x9d");  // &prop;
									ON_SUFFIX(4, "to", "\xe2\x88\x9d");  // &propto;
							CHAR_END()
						CHAR('s') ON_SUFFIX(3, "im", "\xe2\x89\xbe");  // &prsim;
						CHAR('u') ON_SUFFIX(3, "rel", "\xe2\x8a\xb0");  // &prurel;
					CHAR_END()
				CHAR('s')
					CHAR_AT(2)
						CHAR('c') ON_SUFFIX(3, "r", "\xf0\x9d\x93\x85");  // &pscr;
						CHAR('i') ON_LEAF(3, "\xcf\x88");  // &psi;
					CHAR_END()
				CHAR('u') ON_SUFFIX(2, "ncsp", "\xe2\x80\x88");  // &puncsp;
			CHAR_END()
		CHAR('q')
			CHAR_AT(1)
				CHAR('f') ON_SUFFIX(2, "r", "\xf0\x9d\x94\xae");  // &qfr;
				CHAR('i') ON_SUFFIX(2, "nt", "\xe2\xa8\x8c");  // &qint;
				CHAR('o') ON_SUFFIX(2, "pf", "\xf0\x9d\x95\xa2");  // &qopf;
				CHAR('p') ON_SUFFIX(2, "rime", "\xe2\x81\x97");  // &qprime;
				CHAR('s') ON_SUFFIX(2, "cr", "\xf0\x9d\x93\x86");  // &qscr;
				CHAR('u')
					CHAR_AT(2)
						CHAR('a')
							CHAR_AT(3)
								CHAR('t')
									CHAR_AT(4)
										CHAR('e') ON_SUFFIX(5, "rnions", "\xe2\x84\x8d");  // &quaternions;
										CHAR('i') ON_SUFFIX(5, "nt", "\xe2\xa8\x96");  // &quatint;
									CHAR_END()
							CHAR_END()
						CHAR('e')
							CHAR_AT(3)
								CHAR('s')
									CHAR_AT(4)
										CHAR('t')
											ON_LEAF_NOBR(5, "?");  // &quest;
											ON_SUFFIX(5, "eq", "\xe2\x89\x9f");  // &questeq;
									CHAR_END()
							CHAR_END()
						CHAR('o') ON_SUFFIX(3, "t", "\"");  // &quot;
					CHAR_END()
			CHAR_END()
		CHAR('r')
			CHAR_AT(1)
				CHAR('A')
					CHAR_AT(2)
						CHAR('a') ON_SUFFIX(3, "rr", "\xe2\x87\x9b");  // &rAarr;
						CHAR('r') ON_SUFFIX(3, "r", "\xe2\x87\x92");  // &rArr;
						CHAR('t') ON_SUFFIX(3, "ail", "\xe2\xa4\x9c");  // &rAtail;
					CHAR_END()
				CHAR('B') ON_SUFFIX(2, "arr", "\xe2\xa4\x8f");  // &rBarr;
				CHAR('H') ON_SUFFIX(2, "ar", "\xe2\xa5\xa4");  // &rHar;
				CHAR('a')
					CHAR_AT(2)
						CHAR('c')
							CHAR_AT(3)
								CHAR('e') ON_LEAF(4, "\xe2\x88\xbd\xcc\xb1");  // &race;
								CHAR('u') ON_SUFFIX(4, "te", "\xc5\x95");  // &racute;
							CHAR_END()
						CHAR('d') ON_SUFFIX(3, "ic", "\xe2\x88\x9a");  // &radic;
						CHAR('e') ON_SUFFIX(3, "mptyv", "\xe2\xa6\xb3");  // &raemptyv;
						CHAR('n')
							CHAR_AT(3)
								CHAR('g')
									ON_LEAF_NOBR(4, "\xe2\x9f\xa9");  // &rang;
									CHAR_AT(4)
										CHAR('d') ON_LEAF(5, "\xe2\xa6\x92");  // &rangd;
										CHAR('e') ON_LEAF(5, "\xe2\xa6\xa5");  // &range;
										CHAR('l') ON_SUFFIX(5, "e", "\xe2\x9f\xa9");  // &rangle;
									CHAR_END()
							CHAR_END()
						CHAR('q') ON_SUFFIX(3, "uo", "\xc2\xbb");  // &raquo;
						CHAR('r')
							CHAR_AT(3)
								CHAR('r')
									ON_LEAF_NOBR(4, "\xe2\x86\x92");  // &rarr;
									CHAR_AT(4)
										CHAR('a') ON_SUFFIX(5, "p", "\xe2\xa5\xb5");  // &rarrap;
										CHAR('b')
											ON_LEAF_NOBR(5, "\xe2\x87\xa5");  // &rarrb;
											ON_SUFFIX(5, "fs", "\xe2\xa4\xa0");  // &rarrbfs;
										CHAR('c') ON_LEAF(5, "\xe2\xa4\xb3");  // &rarrc;
										CHAR('f') ON_SUFFIX(5, "s", "\xe2\xa4\x9e");  // &rarrfs;
										CHAR('h') ON_SUFFIX(5, "k", "\xe2\x86\xaa");  // &rarrhk;
										CHAR('l') ON_SUFFIX(5, "p", "\xe2\x86\xac");  // &rarrlp;
										CHAR('p') ON_SUFFIX(5, "l", "\xe2\xa5\x85");  // &rarrpl;
										CHAR('s') ON_SUFFIX(5, "im", "\xe2\xa5\xb4");  // &rarrsim;
										CHAR('t') ON_SUFFIX(5, "l", "\xe2\x86\xa3");  // &rarrtl;
										CHAR('w') ON_LEAF(5, "\xe2\x86\x9d");  // &rarrw;
									CHAR_END()
							CHAR_END()
						CHAR('t')
							CHAR_AT(3)
								CHAR('a') ON_SUFFIX(4, "il", "\xe2\xa4\x9a");  // &ratail;
								CHAR('i')
									CHAR_AT(4)
										CHAR('o')
											ON_LEAF_NOBR(5, "\xe2\x88\xb6");  // &ratio;
											ON_SUFFIX(5, "nals", "\xe2\x84\x9a");  // &rationals;
									CHAR_END()
							CHAR_END()
					CHAR_END()
				CHAR('b')
					CHAR_AT(2)
						CHAR('a') ON_SUFFIX(3, "rr", "\xe2\xa4\x8d");  // &rbarr;
						CHAR('b') ON_SUFFIX(3, "rk", "\xe2\x9d\xb3");  // &rbbrk;
						CHAR('r')
							CHAR_AT(3)
								CHAR('a')
									CHAR_AT(4)
										CHAR('c')
											CHAR_AT(5)
												CHAR('e') ON_LEAF(6, "}");  // &rbrace;
												CHAR('k') ON_LEAF(6, "]");  // &rbrack;
											CHAR_END()
									CHAR_END()
								CHAR('k')
									CHAR_AT(4)
										CHAR('e') ON_LEAF(5, "\xe2\xa6\x8c");  // &rbrke;
										CHAR('s')
											CHAR_AT(5)
												CHAR('l')
													CHAR_AT(6)
														CHAR('d') ON_LEAF(7, "\xe2\xa6\x8e");  // &rbrksld;
														CHAR('u') ON_LEAF(7, "\xe2\xa6\x90");  // &rbrkslu;
													CHAR_END()
											CHAR_END()
									CHAR_END()
							CHAR_END()
					CHAR_END()
				CHAR('c')
					CHAR_AT(2)
						CHAR('a') ON_SUFFIX(3, "ron", "\xc5\x99");  // &rcaron;
						CHAR('e')
							CHAR_AT(3)
								CHAR('d') ON_SUFFIX(4, "il", "\xc5\x97");  // &rcedil;
								CHAR('i') ON_SUFFIX(4, "l", "\xe2\x8c\x89");  // &rceil;
							CHAR_END()
						CHAR('u') ON_SUFFIX(3, "b", "}");  // &rcub;
						CHAR('y') ON_LEAF(3, "\xd1\x80");  // &rcy;
					CHAR_END()
				CHAR('d')
					CHAR_AT(2)
						CHAR('c') ON_SUFFIX(3, "a", "\xe2\xa4\xb7");  // &rdca;
						CHAR('l') ON_SUFFIX(3, "dhar", "\xe2\xa5\xa9");  // &rdldhar;
						CHAR('q')
							CHAR_AT(3)
								CHAR('u')
									CHAR_AT(4)
										CHAR('o')
											ON_LEAF_NOBR(5, "\xe2\x80\x9d");  // &rdquo;
											ON_SUFFIX(5, "r", "\xe2\x80\x9d");  // &rdquor;
									CHAR_END()
							CHAR_END()
						CHAR('s') ON_SUFFIX(3, "h", "\xe2\x86\xb3");  // &rdsh;
					CHAR_END()
				CHAR('e')
					CHAR_AT(2)
						CHAR('a')
							CHAR_AT(3)
								CHAR('l')
									ON_LEAF_NOBR(4, "\xe2\x84\x9c");  // &real;
									CHAR_AT(4)
										CHAR('i') ON_SUFFIX(5, "ne", "\xe2\x84\x9b");  // &realine;
										CHAR('p') ON_SUFFIX(5, "art", "\xe2\x84\x9c");  // &realpart;
										CHAR('s') ON_LEAF(5, "\xe2\x84\x9d");  // &reals;
									CHAR_END()
							CHAR_END()
						CHAR('c') ON_SUFFIX(3, "t", "\xe2\x96\xad");  // &rect;
						CHAR('g') ON_LEAF(3, "\xc2\xae");  // &reg;
					CHAR_END()
				CHAR('f')
					CHAR_AT(2)
						CHAR('i') ON_SUFFIX(3, "sht", "\xe2\xa5\xbd");  // &rfisht;
						CHAR('l') ON_SUFFIX(3, "oor", "\xe2\x8c\x8b");  // &rfloor;
						CHAR('r') ON_LEAF(3, "\xf0\x9d\x94\xaf");  // &rfr;
					CHAR_END()
				CHAR('h')
					CHAR_AT(2)
						CHAR('a')
							CHAR_AT(3)
								CHAR('r')
									CHAR_AT(4)
										CHAR('d') ON_LEAF(5, "\xe2\x87\x81");  // &rhard;
										CHAR('u')
											ON_LEAF_NOBR(5, "\xe2\x87\x80");  // &rharu;
											ON_SUFFIX(5, "l", "\xe2\xa5\xac");  // &rharul;
									CHAR_END()
							CHAR_END()
						CHAR('o')
							ON_LEAF_NOBR(3, "\xcf\x81");  // &rho;
							ON_SUFFIX(3, "v", "\xcf\xb1");  // &rhov;
					CHAR_END()
				CHAR('i')
					CHAR_AT(2)
						CHAR('g')
							CHAR_AT(3)
								CHAR('h')
									CHAR_AT(4)
										CHAR('t')
											CHAR_AT(5)
												CHAR('a')
													CHAR_AT(6)
														CHAR('r')
															CHAR_AT(7)
																CHAR('r')
																	CHAR_AT(8)
																		CHAR('o')
																			CHAR_AT(9)
																				CHAR('w')
																					ON_LEAF_NOBR(10, "\xe2\x86\x92");  // &rightarrow;
																					ON_SUFFIX(10, "tail", "\xe2\x86\xa3");  // &rightarrowtail;
																			CHAR_END()
																	CHAR_END()
															CHAR_END()
													CHAR_END()
												CHAR('h')
													CHAR_AT(6)
														CHAR('a')
															CHAR_AT(7)
																CHAR('r')
																	CHAR_AT(8)
																		CHAR('p')
																			CHAR_AT(9)
																				CHAR('o')
																					CHAR_AT(10)
																						CHAR('o')
																							CHAR_AT(11)
																								CHAR('n')
																									CHAR_AT(12)
																										CHAR('d') ON_SUFFIX(13, "own", "\xe2\x87\x81");  // &rightharpoondown;
																										CHAR('u') ON_SUFFIX(13, "p", "\xe2\x87\x80");  // &rightharpoonup;
																									CHAR_END()
																							CHAR_END()
																					CHAR_END()
																			CHAR_END()
																	CHAR_END()
															CHAR_END()
													CHAR_END()
												CHAR('l')
													CHAR_AT(6)
														CHAR('e')
															CHAR_AT(7)
																CHAR('f')
																	CHAR_AT(8)
																		CHAR('t')
																			CHAR_AT(9)
																				CHAR('a') ON_SUFFIX(10, "rrows", "\xe2\x87\x84");  // &rightleftarrows;
																				CHAR('h') ON_SUFFIX(10, "arpoons", "\xe2\x87\x8c");  // &rightleftharpoons;
																			CHAR_END()
																	CHAR_END()
															CHAR_END()
													CHAR_END()
												CHAR('r') ON_SUFFIX(6, "ightarrows", "\xe2\x87\x89");  // &rightrightarrows;
												CHAR('s') ON_SUFFIX(6, "quigarrow", "\xe2\x86\x9d");  // &rightsquigarrow;
												CHAR('t') ON_SUFFIX(6, "hreetimes", "\xe2\x8b\x8c");  // &rightthreetimes;
											CHAR_END()
									CHAR_END()
							CHAR_END()
						CHAR('n') ON_SUFFIX(3, "g", "\xcb\x9a");  // &ring;
						CHAR('s') ON_SUFFIX(3, "ingdotseq", "\xe2\x89\x93");  // &risingdotseq;
					CHAR_END()
				CHAR('l')
					CHAR_AT(2)
						CHAR('a') ON_SUFFIX(3, "rr", "\xe2\x87\x84");  // &rlarr;
						CHAR('h') ON_SUFFIX(3, "ar", "\xe2\x87\x8c");  // &rlhar;
						CHAR('m') ON_LEAF(3, "\xe2\x80\x8f");  // &rlm;
					CHAR_END()
				CHAR('m')
					CHAR_AT(2)
						CHAR('o')
							CHAR_AT(3)
								CHAR('u')
									CHAR_AT(4)
										CHAR('s')
											CHAR_AT(5)
												CHAR('t')
													ON_LEAF_NOBR(6, "\xe2\x8e\xb1");  // &rmoust;
													ON_SUFFIX(6, "ache", "\xe2\x8e\xb1");  // &rmoustache;
											CHAR_END()
									CHAR_END()
							CHAR_END()
					CHAR_END()
				CHAR('n') ON_SUFFIX(2, "mid", "\xe2\xab\xae");  // &rnmid;
				CHAR('o')
					CHAR_AT(2)
						CHAR('a')
							CHAR_AT(3)
								CHAR('n') ON_SUFFIX(4, "g", "\xe2\x9f\xad");  // &roang;
								CHAR('r') ON_SUFFIX(4, "r", "\xe2\x87\xbe");  // &roarr;
							CHAR_END()
						CHAR('b') ON_SUFFIX(3, "rk", "\xe2\x9f\xa7");  // &robrk;
						CHAR('p')
							CHAR_AT(3)
								CHAR('a') ON_SUFFIX(4, "r", "\xe2\xa6\x86");  // &ropar;
								CHAR('f') ON_LEAF(4, "\xf0\x9d\x95\xa3");  // &ropf;
								CHAR('l') ON_SUFFIX(4, "us", "\xe2\xa8\xae");  // &roplus;
							CHAR_END()
						CHAR('t') ON_SUFFIX(3, "imes", "\xe2\xa8\xb5");  // &rotimes;
					CHAR_END()
				CHAR('p')
					CHAR_AT(2)
						CHAR('a')
							CHAR_AT(3)
								CHAR('r')
									ON_LEAF_NOBR(4, ")");  // &rpar;
									ON_SUFFIX(4, "gt", "\xe2\xa6\x94");  // &rpargt;
							CHAR_END()
						CHAR('p') ON_SUFFIX(3, "olint", "\xe2\xa8\x92");  // &rppolint;
					CHAR_END()
				CHAR('r') ON_SUFFIX(2, "arr", "\xe2\x87\x89");  // &rrarr;
				CHAR('s')
					CHAR_AT(2)
						CHAR('a') ON_SUFFIX(3, "quo", "\xe2\x80\xba");  // &rsaquo;
						CHAR('c') ON_SUFFIX(3, "r", "\xf0\x9d\x93\x87");  // &rscr;
						CHAR('h') ON_LEAF(3, "\xe2\x86\xb1");  // &rsh;
						CHAR('q')
							CHAR_AT(3)
								CHAR('b') ON_LEAF(4, "]");  // &rsqb;
								CHAR('u')
									CHAR_AT(4)
										CHAR('o')
											ON_LEAF_NOBR(5, "\xe2\x80\x99");  // &rsquo;
											ON_SUFFIX(5, "r", "\xe2\x80\x99");  // &rsquor;
									CHAR_END()
							CHAR_END()
					CHAR_END()
				CHAR('t')
					CHAR_AT(2)
						CHAR('h') ON_SUFFIX(3, "ree", "\xe2\x8b\x8c");  // &rthree;
						CHAR('i') ON_SUFFIX(3, "mes", "\xe2\x8b\x8a");  // &rtimes;
						CHAR('r')
							CHAR_AT(3)
								CHAR('i')
									ON_LEAF_NOBR(4, "\xe2\x96\xb9");  // &rtri;
									CHAR_AT(4)
										CHAR('e') ON_LEAF(5, "\xe2\x8a\xb5");  // &rtrie;
										CHAR('f') ON_LEAF(5, "\xe2\x96\xb8");  // &rtrif;
										CHAR('l') ON_SUFFIX(5, "tri", "\xe2\xa7\x8e");  // &rtriltri;
									CHAR_END()
							CHAR_END()
					CHAR_END()
				CHAR('u') ON_SUFFIX(2, "luhar", "\xe2\xa5\xa8");  // &ruluhar;
				CHAR('x') ON_LEAF(2, "\xe2\x84\x9e");  // &rx;
			CHAR_END()
		CHAR('s')
			CHAR_AT(1)
				CHAR('a') ON_SUFFIX(2, "cute", "\xc5\x9b");  // &sacute;
				CHAR('b') ON_SUFFIX(2, "quo", "\xe2\x80\x9a");  // &sbquo;
				CHAR('c')
					ON_LEAF_NOBR(2, "\xe2\x89\xbb");  // &sc;
					CHAR_AT(2)
						CHAR('E') ON_LEAF(3, "\xe2\xaa\xb4");  // &scE;
						CHAR('a')
							CHAR_AT(3)
								CHAR('p') ON_LEAF(4, "\xe2\xaa\xb8");  // &scap;
								CHAR('r') ON_SUFFIX(4, "on", "\xc5\xa1");  // &scaron;
							CHAR_END()
						CHAR('c') ON_SUFFIX(3, "ue", "\xe2\x89\xbd");  // &sccue;
						CHAR('e')
							ON_LEAF_NOBR(3, "\xe2\xaa\xb0");  // &sce;
							ON_SUFFIX(3, "dil", "\xc5\x9f");  // &scedil;
						CHAR('i') ON_SUFFIX(3, "rc", "\xc5\x9d");  // &scirc;
						CHAR('n')
							CHAR_AT(3)
								CHAR('E') ON_LEAF(4, "\xe2\xaa\xb6");  // &scnE;
								CHAR('a') ON_SUFFIX(4, "p", "\xe2\xaa\xba");  // &scnap;
								CHAR('s') ON_SUFFIX(4, "im", "\xe2\x8b\xa9");  // &scnsim;
							CHAR_END()
						CHAR('p') ON_SUFFIX(3, "olint", "\xe2\xa8\x93");  // &scpolint;
						CHAR('s') ON_SUFFIX(3, "im", "\xe2\x89\xbf");  // &scsim;
						CHAR('y') ON_LEAF(3, "\xd1\x81");  // &scy;
					CHAR_END()
				CHAR('d')
					CHAR_AT(2)
						CHAR('o')
							CHAR_AT(3)
								CHAR('t')
									ON_LEAF_NOBR(4, "\xe2\x8b\x85");  // &sdot;
									CHAR_AT(4)
										CHAR('b') ON_LEAF(5, "\xe2\x8a\xa1");  // &sdotb;
										CHAR('e') ON_LEAF(5, "\xe2\xa9\xa6");  // &sdote;
									CHAR_END()
							CHAR_END()
					CHAR_END()
				CHAR('e')
					CHAR_AT(2)
						CHAR('A') ON_SUFFIX(3, "rr", "\xe2\x87\x98");  // &seArr;
						CHAR('a')
							CHAR_AT(3)
								CHAR('r')
									CHAR_AT(4)
										CHAR('h') ON_SUFFIX(5, "k", "\xe2\xa4\xa5");  // &searhk;
										CHAR('r')
											ON_LEAF_NOBR(5, "\xe2\x86\x98");  // &searr;
											ON_SUFFIX(5, "ow", "\xe2\x86\x98");  // &searrow;
									CHAR_END()
							CHAR_END()
						CHAR('c') ON_SUFFIX(3, "t", "\xc2\xa7");  // &sect;
						CHAR('m') ON_SUFFIX(3, "i", ";");  // &semi;
						CHAR('s') ON_SUFFIX(3, "war", "\xe2\xa4\xa9");  // &seswar;
						CHAR('t')
							CHAR_AT(3)
								CHAR('m')
									CHAR_AT(4)
										CHAR('i') ON_SUFFIX(5, "nus", "\xe2\x88\x96");  // &setminus;
										CHAR('n') ON_LEAF(5, "\xe2\x88\x96");  // &setmn;
									CHAR_END()
							CHAR_END()
						CHAR('x') ON_SUFFIX(3, "t", "\xe2\x9c\xb6");  // &sext;
					CHAR_END()
				CHAR('f')
					CHAR_AT(2)
						CHAR('r')
							ON_LEAF_NOBR(3, "\xf0\x9d\x94\xb0");  // &sfr;
							ON_SUFFIX(3, "own", "\xe2\x8c\xa2");  // &sfrown;
					CHAR_END()
				CHAR('h')
					CHAR_AT(2)
						CHAR('a') ON_SUFFIX(3, "rp", "\xe2\x99\xaf");  // &sharp;
						CHAR('c')
							CHAR_AT(3)
								CHAR('h') ON_SUFFIX(4, "cy", "\xd1\x89");  // &shchcy;
								CHAR('y') ON_LEAF(4, "\xd1\x88");  // &shcy;
							CHAR_END()
						CHAR('o')
							CHAR_AT(3)
								CHAR('r')
									CHAR_AT(4)
										CHAR('t')
											CHAR_AT(5)
												CHAR('m') ON_SUFFIX(6, "id", "\xe2\x88\xa3");  // &shortmid;
												CHAR('p') ON_SUFFIX(6, "arallel", "\xe2\x88\xa5");  // &shortparallel;
											CHAR_END()
									CHAR_END()
							CHAR_END()
						CHAR('y') ON_LEAF(3, "\xc2\xad");  // &shy;
					CHAR_END()
				CHAR('i')
					CHAR_AT(2)
						CHAR('g')
							CHAR_AT(3)
								CHAR('m')
									CHAR_AT(4)
										CHAR('a')
											ON_LEAF_NOBR(5, "\xcf\x83");  // &sigma;
											CHAR_AT(5)
												CHAR('f') ON_LEAF(6, "\xcf\x82");  // &sigmaf;
												CHAR('v') ON_LEAF(6, "\xcf\x82");  // &sigmav;
											CHAR_END()
									CHAR_END()
							CHAR_END()
						CHAR('m')
							ON_LEAF_NOBR(3, "\xe2\x88\xbc");  // &sim;
							CHAR_AT(3)
								CHAR('d') ON_SUFFIX(4, "ot", "\xe2\xa9\xaa");  // &simdot;
								CHAR('e')
									ON_LEAF_NOBR(4, "\xe2\x89\x83");  // &sime;
									ON_SUFFIX(4, "q", "\xe2\x89\x83");  // &simeq;
								CHAR('g')
									ON_LEAF_NOBR(4, "\xe2\xaa\x9e");  // &simg;
									ON_SUFFIX(4, "E", "\xe2\xaa\xa0");  // &simgE;
								CHAR('l')
									ON_LEAF_NOBR(4, "\xe2\xaa\x9d");  // &siml;
									ON_SUFFIX(4, "E", "\xe2\xaa\x9f");  // &simlE;
								CHAR('n') ON_SUFFIX(4, "e", "\xe2\x89\x86");  // &simne;
								CHAR('p') ON_SUFFIX(4, "lus", "\xe2\xa8\xa4");  // &simplus;
								CHAR('r') ON_SUFFIX(4, "arr", "\xe2\xa5\xb2");  // &simrarr;
							CHAR_END()
					CHAR_END()
				CHAR('l') ON_SUFFIX(2, "arr", "\xe2\x86\x90");  // &slarr;
				CHAR('m')
					CHAR_AT(2)
						CHAR('a')
							CHAR_AT(3)
								CHAR('l') ON_SUFFIX(4, "lsetminus", "\xe2\x88\x96");  // &smallsetminus;
								CHAR('s') ON_SUFFIX(4, "hp", "\xe2\xa8\xb3");  // &smashp;
							CHAR_END()
						CHAR('e') ON_SUFFIX(3, "parsl", "\xe2\xa7\xa4");  // &smeparsl;
						CHAR('i')
							CHAR_AT(3)
								CHAR('d') ON_LEAF(4, "\xe2\x88\xa3");  // &smid;
								CHAR('l') ON_SUFFIX(4, "e", "\xe2\x8c\xa3");  // &smile;
							CHAR_END()
						CHAR('t')
							ON_LEAF_NOBR(3, "\xe2\xaa\xaa");  // &smt;
							CHAR_AT(3)
								CHAR('e')
									ON_LEAF_NOBR(4, "\xe2\xaa\xac");  // &smte;
									ON_SUFFIX(4, "s", "\xe2\xaa\xac\xef\xb8\x80");  // &smtes;
							CHAR_END()
					CHAR_END()
				CHAR('o')
					CHAR_AT(2)
						CHAR('f') ON_SUFFIX(3, "tcy", "\xd1\x8c");  // &softcy;
						CHAR('l')
							ON_LEAF_NOBR(3, "/");  // &sol;
							CHAR_AT(3)
								CHAR('b')
									ON_LEAF_NOBR(4, "\xe2\xa7\x84");  // &solb;
									ON_SUFFIX(4, "ar", "\xe2\x8c\xbf");  // &solbar;
							CHAR_END()
						CHAR('p') ON_SUFFIX(3, "f", "\xf0\x9d\x95\xa4");  // &sopf;
					CHAR_END()
				CHAR('p')
					CHAR_AT(2)
						CHAR('a')
							CHAR_AT(3)
								CHAR('d')
									CHAR_AT(4)
										CHAR('e')
											CHAR_AT(5)
												CHAR('s')
													ON_LEAF_NOBR(6, "\xe2\x99\xa0");  // &spades;
													ON_SUFFIX(6, "uit", "\xe2\x99\xa0");  // &spadesuit;
											CHAR_END()
									CHAR_END()
								CHAR('r') ON_LEAF(4, "\xe2\x88\xa5");  // &spar;
							CHAR_END()
					CHAR_END()
				CHAR('q')
					CHAR_AT(2)
						CHAR('c')
							CHAR_AT(3)
								CHAR('a')
									CHAR_AT(4)
										CHAR('p')
											ON_LEAF_NOBR(5, "\xe2\x8a\x93");  // &sqcap;
											ON_SUFFIX(5, "s", "\xe2\x8a\x93\xef\xb8\x80");  // &sqcaps;
									CHAR_END()
								CHAR('u')
									CHAR_AT(4)
										CHAR('p')
											ON_LEAF_NOBR(5, "\xe2\x8a\x94");  // &sqcup;
											ON_SUFFIX(5, "s", "\xe2\x8a\x94\xef\xb8\x80");  // &sqcups;
									CHAR_END()
							CHAR_END()
						CHAR('s')
							CHAR_AT(3)
								CHAR('u')
									CHAR_AT(4)
										CHAR('b')
											ON_LEAF_NOBR(5, "\xe2\x8a\x8f");  // &sqsub;
											CHAR_AT(5)
												CHAR('e') ON_LEAF(6, "\xe2\x8a\x91");  // &sqsube;
												CHAR('s')
													CHAR_AT(6)
														CHAR('e')
															CHAR_AT(7)
																CHAR('t')
																	ON_LEAF_NOBR(8, "\xe2\x8a\x8f");  // &sqsubset;
																	ON_SUFFIX(8, "eq", "\xe2\x8a\x91");  // &sqsubseteq;
															CHAR_END()
													CHAR_END()
											CHAR_END()
										CHAR('p')
											ON_LEAF_NOBR(5, "\xe2\x8a\x90");  // &sqsup;
											CHAR_AT(5)
												CHAR('e') ON_LEAF(6, "\xe2\x8a\x92");  // &sqsupe;
												CHAR('s')
													CHAR_AT(6)
														CHAR('e')
															CHAR_AT(7)
																CHAR('t')
																	ON_LEAF_NOBR(8, "\xe2\x8a\x90");  // &sqsupset;
																	ON_SUFFIX(8, "eq", "\xe2\x8a\x92");  // &sqsupseteq;
															CHAR_END()
													CHAR_END()
											CHAR_END()
									CHAR_END()
							CHAR_END()
						CHAR('u')
							ON_LEAF_NOBR(3, "\xe2\x96\xa1");  // &squ;
							CHAR_AT(3)
								CHAR('a')
									CHAR_AT(4)
										CHAR('r')
											CHAR_AT(5)
												CHAR('e') ON_LEAF(6, "\xe2\x96\xa1");  // &square;
												CHAR('f') ON_LEAF(6, "\xe2\x96\xaa");  // &squarf;
											CHAR_END()
									CHAR_END()
								CHAR('f') ON_LEAF(4, "\xe2\x96\xaa");  // &squf;
							CHAR_END()
					CHAR_END()
				CHAR('r') ON_SUFFIX(2, "arr", "\xe2\x86\x92");  // &srarr;
				CHAR('s')
					CHAR_AT(2)
						CHAR('c') ON_SUFFIX(3, "r", "\xf0\x9d\x93\x88");  // &sscr;
						CHAR('e') ON_SUFFIX(3, "tmn", "\xe2\x88\x96");  // &ssetmn;
						CHAR('m') ON_SUFFIX(3, "ile", "\xe2\x8c\xa3");  // &ssmile;
						CHAR('t') ON_SUFFIX(3, "arf", "\xe2\x8b\x86");  // &sstarf;
					CHAR_END()
				CHAR('t')
					CHAR_AT(2)
						CHAR('a')
							CHAR_AT(3)
								CHAR('r')
									ON_LEAF_NOBR(4, "\xe2\x98\x86");  // &star;
									ON_SUFFIX(4, "f", "\xe2\x98\x85");  // &starf;
							CHAR_END()
						CHAR('r')
							CHAR_AT(3)
								CHAR('a')
									CHAR_AT(4)
										CHAR('i')
											CHAR_AT(5)
												CHAR('g')
													CHAR_AT(6)
														CHAR('h')
															CHAR_AT(7)
																CHAR('t')
																	CHAR_AT(8)
																		CHAR('e') ON_SUFFIX(9, "psilon", "\xcf\xb5");  // &straightepsilon;
																		CHAR('p') ON_SUFFIX(9, "hi", "\xcf\x95");  // &straightphi;
																	CHAR_END()
															CHAR_END()
													CHAR_END()
											CHAR_END()
									CHAR_END()
								CHAR('n') ON_SUFFIX(4, "s", "\xc2\xaf");  // &strns;
							CHAR_END()
					CHAR_END()
				CHAR('u')
					CHAR_AT(2)
						CHAR('b')
							ON_LEAF_NOBR(3, "\xe2\x8a\x82");  // &sub;
							CHAR_AT(3)
								CHAR('E') ON_LEAF(4, "\xe2\xab\x85");  // &subE;
								CHAR('d') ON_SUFFIX(4, "ot", "\xe2\xaa\xbd");  // &subdot;
								CHAR('e')
									ON_LEAF_NOBR(4, "\xe2\x8a\x86");  // &sube;
									ON_SUFFIX(4, "dot", "\xe2\xab\x83");  // &subedot;
								CHAR('m') ON_SUFFIX(4, "ult", "\xe2\xab\x81");  // &submult;
								CHAR('n')
									CHAR_AT(4)
										CHAR('E') ON_LEAF(5, "\xe2\xab\x8b");  // &subnE;
										CHAR('e') ON_LEAF(5, "\xe2\x8a\x8a");  // &subne;
									CHAR_END()
								CHAR('p') ON_SUFFIX(4, "lus", "\xe2\xaa\xbf");  // &subplus;
								CHAR('r') ON_SUFFIX(4, "arr", "\xe2\xa5\xb9");  // &subrarr;
								CHAR('s')
									CHAR_AT(4)
										CHAR('e')
											CHAR_AT(5)
												CHAR('t')
													ON_LEAF_NOBR(6, "\xe2\x8a\x82");  // &subset;
													CHAR_AT(6)
														CHAR('e')
															CHAR_AT(7)
																CHAR('q')
																	ON_LEAF_NOBR(8, "\xe2\x8a\x86");  // &subseteq;
																	ON_SUFFIX(8, "q", "\xe2\xab\x85");  // &subseteqq;
															CHAR_END()
														CHAR('n')
															CHAR_AT(7)
																CHAR('e')
																	CHAR_AT(8)
																		CHAR('q')
																			ON_LEAF_NOBR(9, "\xe2\x8a\x8a");  // &subsetneq;
																			ON_SUFFIX(9, "q", "\xe2\xab\x8b");  // &subsetneqq;
																	CHAR_END()
															CHAR_END()
													CHAR_END()
											CHAR_END()
										CHAR('i') ON_SUFFIX(5, "m", "\xe2\xab\x87");  // &subsim;
										CHAR('u')
											CHAR_AT(5)
												CHAR('b') ON_LEAF(6, "\xe2\xab\x95");  // &subsub;
												CHAR('p') ON_LEAF(6, "\xe2\xab\x93");  // &subsup;
											CHAR_END()
									CHAR_END()
							CHAR_END()
						CHAR('c')
							CHAR_AT(3)
								CHAR('c')
									ON_LEAF_NOBR(4, "\xe2\x89\xbb");  // &succ;
									CHAR_AT(4)
										CHAR('a') ON_SUFFIX(5, "pprox", "\xe2\xaa\xb8");  // &succapprox;
										CHAR('c') ON_SUFFIX(5, "urlyeq", "\xe2\x89\xbd");  // &succcurlyeq;
										CHAR('e') ON_SUFFIX(5, "q", "\xe2\xaa\xb0");  // &succeq;
										CHAR('n')
											CHAR_AT(5)
												CHAR('a') ON_SUFFIX(6, "pprox", "\xe2\xaa\xba");  // &succnapprox;
												CHAR('e') ON_SUFFIX(6, "qq", "\xe2\xaa\xb6");  // &succneqq;
												CHAR('s') ON_SUFFIX(6, "im", "\xe2\x8b\xa9");  // &succnsim;
											CHAR_END()
										CHAR('s') ON_SUFFIX(5, "im", "\xe2\x89\xbf");  // &succsim;
									CHAR_END()
							CHAR_END()
						CHAR('m') ON_LEAF(3, "\xe2\x88\x91");  // &sum;
						CHAR('n') ON_SUFFIX(3, "g", "\xe2\x99\xaa");  // &sung;
						CHAR('p')
							ON_LEAF_NOBR(3, "\xe2\x8a\x83");  // &sup;
							CHAR_AT(3)
								CHAR('1') ON_LEAF(4, "\xc2\xb9");  // &sup1;
								CHAR('2') ON_LEAF(4, "\xc2\xb2");  // &sup2;
								CHAR('3') ON_LEAF(4, "\xc2\xb3");  // &sup3;
								CHAR('E') ON_LEAF(4, "\xe2\xab\x86");  // &supE;
								CHAR('d')
									CHAR_AT(4)
										CHAR('o') ON_SUFFIX(5, "t", "\xe2\xaa\xbe");  // &supdot;
										CHAR('s') ON_SUFFIX(5, "ub", "\xe2\xab\x98");  // &supdsub;
									CHAR_END()
								CHAR('e')
									ON_LEAF_NOBR(4, "\xe2\x8a\x87");  // &supe;
									ON_SUFFIX(4, "dot", "\xe2\xab\x84");  // &supedot;
								CHAR('h')
									CHAR_AT(4)
										CHAR('s')
											CHAR_AT(5)
												CHAR('o') ON_SUFFIX(6, "l", "\xe2\x9f\x89");  // &suphsol;
												CHAR('u') ON_SUFFIX(6, "b", "\xe2\xab\x97");  // &suphsub;
											CHAR_END()
									CHAR_END()
								CHAR('l') ON_SUFFIX(4, "arr", "\xe2\xa5\xbb");  // &suplarr;
								CHAR('m') ON_SUFFIX(4, "ult", "\xe2\xab\x82");  // &supmult;
								CHAR('n')
									CHAR_AT(4)
										CHAR('E') ON_LEAF(5, "\xe2\xab\x8c");  // &supnE;
										CHAR('e') ON_LEAF(5, "\xe2\x8a\x8b");  // &supne;
									CHAR_END()
								CHAR('p') ON_SUFFIX(4, "lus", "\xe2\xab\x80");  // &supplus;
								CHAR('s')
									CHAR_AT(4)
										CHAR('e')
											CHAR_AT(5)
												CHAR('t')
													ON_LEAF_NOBR(6, "\xe2\x8a\x83");  // &supset;
													CHAR_AT(6)
														CHAR('e')
															CHAR_AT(7)
																CHAR('q')
																	ON_LEAF_NOBR(8, "\xe2\x8a\x87");  // &supseteq;
																	ON_SUFFIX(8, "q", "\xe2\xab\x86");  // &supseteqq;
															CHAR_END()
														CHAR('n')
															CHAR_AT(7)
																CHAR('e')
																	CHAR_AT(8)
																		CHAR('q')
																			ON_LEAF_NOBR(9, "\xe2\x8a\x8b");  // &supsetneq;
																			ON_SUFFIX(9, "q", "\xe2\xab\x8c");  // &supsetneqq;
																	CHAR_END()
															CHAR_END()
													CHAR_END()
											CHAR_END()
										CHAR('i') ON_SUFFIX(5, "m", "\xe2\xab\x88");  // &supsim;
										CHAR('u')
											CHAR_AT(5)
												CHAR('b') ON_LEAF(6, "\xe2\xab\x94");  // &supsub;
												CHAR('p') ON_LEAF(6, "\xe2\xab\x96");  // &supsup;
											CHAR_END()
									CHAR_END()
							CHAR_END()
					CHAR_END()
				CHAR('w')
					CHAR_AT(2)
						CHAR('A') ON_SUFFIX(3, "rr", "\xe2\x87\x99");  // &swArr;
						CHAR('a')
							CHAR_AT(3)
								CHAR('r')
									CHAR_AT(4)
										CHAR('h') ON_SUFFIX(5, "k", "\xe2\xa4\xa6");  // &swarhk;
										CHAR('r')
											ON_LEAF_NOBR(5, "\xe2\x86\x99");  // &swarr;
											ON_SUFFIX(5, "ow", "\xe2\x86\x99");  // &swarrow;
									CHAR_END()
							CHAR_END()
						CHAR('n') ON_SUFFIX(3, "war", "\xe2\xa4\xaa");  // &swnwar;
					CHAR_END()
				CHAR('z') ON_SUFFIX(2, "lig", "\xc3\x9f");  // &szlig;
			CHAR_END()
		CHAR('t')
			CHAR_AT(1)
				CHAR('a')
					CHAR_AT(2)
						CHAR('r') ON_SUFFIX(3, "get", "\xe2\x8c\x96");  // &target;
						CHAR('u') ON_LEAF(3, "\xcf\x84");  // &tau;
					CHAR_END()
				CHAR('b') ON_SUFFIX(2, "rk", "\xe2\x8e\xb4");  // &tbrk;
				CHAR('c')
					CHAR_AT(2)
						CHAR('a') ON_SUFFIX(3, "ron", "\xc5\xa5");  // &tcaron;
						CHAR('e') ON_SUFFIX(3, "dil", "\xc5\xa3");  // &tcedil;
						CHAR('y') ON_LEAF(3, "\xd1\x82");  // &tcy;
					CHAR_END()
				CHAR('d') ON_SUFFIX(2, "ot", "\xe2\x83\x9b");  // &tdot;
				CHAR('e') ON_SUFFIX(2, "lrec", "\xe2\x8c\x95");  // &telrec;
				CHAR('f') ON_SUFFIX(2, "r", "\xf0\x9d\x94\xb1");  // &tfr;
				CHAR('h')
					CHAR_AT(2)
						CHAR('e')
							CHAR_AT(3)
								CHAR('r')
									CHAR_AT(4)
										CHAR('e')
											CHAR_AT(5)
												CHAR('4') ON_LEAF(6, "\xe2\x88\xb4");  // &there4;
												CHAR('f') ON_SUFFIX(6, "ore", "\xe2\x88\xb4");  // &therefore;
											CHAR_END()
									CHAR_END()
								CHAR('t')
									CHAR_AT(4)
										CHAR('a')
											ON_LEAF_NOBR(5, "\xce\xb8");  // &theta;
											CHAR_AT(5)
												CHAR('s') ON_SUFFIX(6, "ym", "\xcf\x91");  // &thetasym;
												CHAR('v') ON_LEAF(6, "\xcf\x91");  // &thetav;
											CHAR_END()
									CHAR_END()
							CHAR_END()
						CHAR('i')
							CHAR_AT(3)
								CHAR('c')
									CHAR_AT(4)
										CHAR('k')
											CHAR_AT(5)
												CHAR('a') ON_SUFFIX(6, "pprox", "\xe2\x89\x88");  // &thickapprox;
												CHAR('s') ON_SUFFIX(6, "im", "\xe2\x88\xbc");  // &thicksim;
											CHAR_END()
									CHAR_END()
								CHAR('n') ON_SUFFIX(4, "sp", "\xe2\x80\x89");  // &thinsp;
							CHAR_END()
						CHAR('k')
							CHAR_AT(3)
								CHAR('a') ON_SUFFIX(4, "p", "\xe2\x89\x88");  // &thkap;
								CHAR('s') ON_SUFFIX(4, "im", "\xe2\x88\xbc");  // &thksim;
							CHAR_END()
						CHAR('o') ON_SUFFIX(3, "rn", "\xc3\xbe");  // &thorn;
					CHAR_END()
				CHAR('i')
					CHAR_AT(2)
						CHAR('l') ON_SUFFIX(3, "de", "\xcb\x9c");  // &tilde;
						CHAR('m')
							CHAR_AT(3)
								CHAR('e')
									CHAR_AT(4)
										CHAR('s')
											ON_LEAF_NOBR(5, "\xc3\x97");  // &times;
											CHAR_AT(5)
												CHAR('b')
													ON_LEAF_NOBR(6, "\xe2\x8a\xa0");  // &timesb;
													ON_SUFFIX(6, "ar", "\xe2\xa8\xb1");  // &timesbar;
												CHAR('d') ON_LEAF(6, "\xe2\xa8\xb0");  // &timesd;
											CHAR_END()
									CHAR_END()
							CHAR_END()
						CHAR('n') ON_SUFFIX(3, "t", "\xe2\x88\xad");  // &tint;
					CHAR_END()
				CHAR('o')
					CHAR_AT(2)
						CHAR('e') ON_SUFFIX(3, "a", "\xe2\xa4\xa8");  // &toea;
						CHAR('p')
							ON_LEAF_NOBR(3, "\xe2\x8a\xa4");  // &top;
							CHAR_AT(3)
								CHAR('b') ON_SUFFIX(4, "ot", "\xe2\x8c\xb6");  // &topbot;
								CHAR('c') ON_SUFFIX(4, "ir", "\xe2\xab\xb1");  // &topcir;
								CHAR('f')
									ON_LEAF_NOBR(4, "\xf0\x9d\x95\xa5");  // &topf;
									ON_SUFFIX(4, "ork", "\xe2\xab\x9a");  // &topfork;
							CHAR_END()
						CHAR('s') ON_SUFFIX(3, "a", "\xe2\xa4\xa9");  // &tosa;
					CHAR_END()
				CHAR('p') ON_SUFFIX(2, "rime", "\xe2\x80\xb4");  // &tprime;
				CHAR('r')
					CHAR_AT(2)
						CHAR('a') ON_SUFFIX(3, "de", "\xe2\x84\xa2");  // &trade;
						CHAR('i')
							CHAR_AT(3)
								CHAR('a')
									CHAR_AT(4)
										CHAR('n')
											CHAR_AT(5)
												CHAR('g')
													CHAR_AT(6)
														CHAR('l')
															CHAR_AT(7)
																CHAR('e')
																	ON_LEAF_NOBR(8, "\xe2\x96\xb5");  // &triangle;
																	CHAR_AT(8)
																		CHAR('d') ON_SUFFIX(9, "own", "\xe2\x96\xbf");  // &triangledown;
																		CHAR('l')
																			CHAR_AT(9)
																				CHAR('e')
																					CHAR_AT(10)
																						CHAR('f')
																							CHAR_AT(11)
																								CHAR('t')
																									ON_LEAF_NOBR(12, "\xe2\x97\x83");  // &triangleleft;
																									ON_SUFFIX(12, "eq", "\xe2\x8a\xb4");  // &trianglelefteq;
																							CHAR_END()
																					CHAR_END()
																			CHAR_END()
																		CHAR('q') ON_LEAF(9, "\xe2\x89\x9c");  // &triangleq;
																		CHAR('r')
																			CHAR_AT(9)
																				CHAR('i')
																					CHAR_AT(10)
																						CHAR('g')
																							CHAR_AT(11)
																								CHAR('h')
																									CHAR_AT(12)
																										CHAR('t')
																											ON_LEAF_NOBR(13, "\xe2\x96\xb9");  // &triangleright;
																											ON_SUFFIX(13, "eq", "\xe2\x8a\xb5");  // &trianglerighteq;
																									CHAR_END()
																							CHAR_END()
																					CHAR_END()
																			CHAR_END()
																	CHAR_END()
															CHAR_END()
													CHAR_END()
											CHAR_END()
									CHAR_END()
								CHAR('d') ON_SUFFIX(4, "ot", "\xe2\x97\xac");  // &tridot;
								CHAR('e') ON_LEAF(4, "\xe2\x89\x9c");  // &trie;
								CHAR('m') ON_SUFFIX(4, "inus", "\xe2\xa8\xba");  // &triminus;
								CHAR('p') ON_SUFFIX(4, "lus", "\xe2\xa8\xb9");  // &triplus;
								CHAR('s') ON_SUFFIX(4, "b", "\xe2\xa7\x8d");  // &trisb;
								CHAR('t') ON_SUFFIX(4, "ime", "\xe2\xa8\xbb");  // &tritime;
							CHAR_END()
						CHAR('p') ON_SUFFIX(3, "ezium", "\xe2\x8f\xa2");  // &trpezium;
					CHAR_END()
				CHAR('s')
					CHAR_AT(2)
						CHAR('c')
							CHAR_AT(3)
								CHAR('r') ON_LEAF(4, "\xf0\x9d\x93\x89");  // &tscr;
								CHAR('y') ON_LEAF(4, "\xd1\x86");  // &tscy;
							CHAR_END()
						CHAR('h') ON_SUFFIX(3, "cy", "\xd1\x9b");  // &tshcy;
						CHAR('t') ON_SUFFIX(3, "rok", "\xc5\xa7");  // &tstrok;
					CHAR_END()
				CHAR('w')
					CHAR_AT(2)
						CHAR('i') ON_SUFFIX(3, "xt", "\xe2\x89\xac");  // &twixt;
						CHAR('o')
							CHAR_AT(3)
								CHAR('h')
									CHAR_AT(4)
										CHAR('e')
											CHAR_AT(5)
												CHAR('a')
													CHAR_AT(6)
														CHAR('d')
															CHAR_AT(7)
																CHAR('l') ON_SUFFIX(8, "eftarrow", "\xe2\x86\x9e");  // &twoheadleftarrow;
																CHAR('r') ON_SUFFIX(8, "ightarrow", "\xe2\x86\xa0");  // &twoheadrightarrow;
															CHAR_END()
													CHAR_END()
											CHAR_END()
									CHAR_END()
							CHAR_END()
					CHAR_END()
			CHAR_END()
		CHAR('u')
			CHAR_AT(1)
				CHAR('A') ON_SUFFIX(2, "rr", "\xe2\x87\x91");  // &uArr;
				CHAR('H') ON_SUFFIX(2, "ar", "\xe2\xa5\xa3");  // &uHar;
				CHAR('a')
					CHAR_AT(2)
						CHAR('c') ON_SUFFIX(3, "ute", "\xc3\xba");  // &uacute;
						CHAR('r') ON_SUFFIX(3, "r", "\xe2\x86\x91");  // &uarr;
					CHAR_END()
				CHAR('b')
					CHAR_AT(2)
						CHAR('r')
							CHAR_AT(3)
								CHAR('c') ON_SUFFIX(4, "y", "\xd1\x9e");  // &ubrcy;
								CHAR('e') ON_SUFFIX(4, "ve", "\xc5\xad");  // &ubreve;
							CHAR_END()
					CHAR_END()
				CHAR('c')
					CHAR_AT(2)
						CHAR('i') ON_SUFFIX(3, "rc", "\xc3\xbb");  // &ucirc;
						CHAR('y') ON_LEAF(3, "\xd1\x83");  // &ucy;
					CHAR_END()
				CHAR('d')
					CHAR_AT(2)
						CHAR('a') ON_SUFFIX(3, "rr", "\xe2\x87\x85");  // &udarr;
						CHAR('b') ON_SUFFIX(3, "lac", "\xc5\xb1");  // &udblac;
						CHAR('h') ON_SUFFIX(3, "ar", "\xe2\xa5\xae");  // &udhar;
					CHAR_END()
				CHAR('f')
					CHAR_AT(2)
						CHAR('i') ON_SUFFIX(3, "sht", "\xe2\xa5\xbe");  // &ufisht;
						CHAR('r') ON_LEAF(3, "\xf0\x9d\x94\xb2");  // &ufr;
					CHAR_END()
				CHAR('g') ON_SUFFIX(2, "rave", "\xc3\xb9");  // &ugrave;
				CHAR('h')
					CHAR_AT(2)
						CHAR('a')
							CHAR_AT(3)
								CHAR('r')
									CHAR_AT(4)
										CHAR('l') ON_LEAF(5, "\xe2\x86\xbf");  // &uharl;
										CHAR('r') ON_LEAF(5, "\xe2\x86\xbe");  // &uharr;
									CHAR_END()
							CHAR_END()
						CHAR('b') ON_SUFFIX(3, "lk", "\xe2\x96\x80");  // &uhblk;
					CHAR_END()
				CHAR('l')
					CHAR_AT(2)
						CHAR('c')
							CHAR_AT(3)
								CHAR('o')
									CHAR_AT(4)
										CHAR('r')
											CHAR_AT(5)
												CHAR('n')
													ON_LEAF_NOBR(6, "\xe2\x8c\x9c");  // &ulcorn;
													ON_SUFFIX(6, "er", "\xe2\x8c\x9c");  // &ulcorner;
											CHAR_END()
									CHAR_END()
								CHAR('r') ON_SUFFIX(4, "op", "\xe2\x8c\x8f");  // &ulcrop;
							CHAR_END()
						CHAR('t') ON_SUFFIX(3, "ri", "\xe2\x97\xb8");  // &ultri;
					CHAR_END()
				CHAR('m')
					CHAR_AT(2)
						CHAR('a') ON_SUFFIX(3, "cr", "\xc5\xab");  // &umacr;
						CHAR('l') ON_LEAF(3, "\xc2\xa8");  // &uml;
					CHAR_END()
				CHAR('o')
					CHAR_AT(2)
						CHAR('g') ON_SUFFIX(3, "on", "\xc5\xb3");  // &uogon;
						CHAR('p') ON_SUFFIX(3, "f", "\xf0\x9d\x95\xa6");  // &uopf;
					CHAR_END()
				CHAR('p')
					CHAR_AT(2)
						CHAR('a') ON_SUFFIX(3, "rrow", "\xe2\x86\x91");  // &uparrow;
						CHAR('d') ON_SUFFIX(3, "ownarrow", "\xe2\x86\x95");  // &updownarrow;
						CHAR('h')
							CHAR_AT(3)
								CHAR('a')
									CHAR_AT(4)
										CHAR('r')
											CHAR_AT(5)
												CHAR('p')
													CHAR_AT(6)
														CHAR('o')
															CHAR_AT(7)
																CHAR('o')
																	CHAR_AT(8)
																		CHAR('n')
																			CHAR_AT(9)
																				CHAR('l') ON_SUFFIX(10, "eft", "\xe2\x86\xbf");  // &upharpoonleft;
																				CHAR('r') ON_SUFFIX(10, "ight", "\xe2\x86\xbe");  // &upharpoonright;
																			CHAR_END()
																	CHAR_END()
															CHAR_END()
													CHAR_END()
											CHAR_END()
									CHAR_END()
							CHAR_END()
						CHAR('l') ON_SUFFIX(3, "us", "\xe2\x8a\x8e");  // &uplus;
						CHAR('s')
							CHAR_AT(3)
								CHAR('i')
									ON_LEAF_NOBR(4, "\xcf\x85");  // &upsi;
									CHAR_AT(4)
										CHAR('h') ON_LEAF(5, "\xcf\x92");  // &upsih;
										CHAR('l') ON_SUFFIX(5, "on", "\xcf\x85");  // &upsilon;
									CHAR_END()
							CHAR_END()
						CHAR('u') ON_SUFFIX(3, "parrows", "\xe2\x87\x88");  // &upuparrows;
					CHAR_END()
				CHAR('r')
					CHAR_AT(2)
						CHAR('c')
							CHAR_AT(3)
								CHAR('o')
									CHAR_AT(4)
										CHAR('r')
											CHAR_AT(5)
												CHAR('n')
													ON_LEAF_NOBR(6, "\xe2\x8c\x9d");  // &urcorn;
													ON_SUFFIX(6, "er", "\xe2\x8c\x9d");  // &urcorner;
											CHAR_END()
									CHAR_END()
								CHAR('r') ON_SUFFIX(4, "op", "\xe2\x8c\x8e");  // &urcrop;
							CHAR_END()
						CHAR('i') ON_SUFFIX(3, "ng", "\xc5\xaf");  // &uring;
						CHAR('t') ON_SUFFIX(3, "ri", "\xe2\x97\xb9");  // &urtri;
					CHAR_END()
				CHAR('s') ON_SUFFIX(2, "cr", "\xf0\x9d\x93\x8a");  // &uscr;
				CHAR('t')
					CHAR_AT(2)
						CHAR('d') ON_SUFFIX(3, "ot", "\xe2\x8b\xb0");  // &utdot;
						CHAR('i') ON_SUFFIX(3, "lde", "\xc5\xa9");  // &utilde;
						CHAR('r')
							CHAR_AT(3)
								CHAR('i')
									ON_LEAF_NOBR(4, "\xe2\x96\xb5");  // &utri;
									ON_SUFFIX(4, "f", "\xe2\x96\xb4");  // &utrif;
							CHAR_END()
					CHAR_END()
				CHAR('u')
					CHAR_AT(2)
						CHAR('a') ON_SUFFIX(3, "rr", "\xe2\x87\x88");  // &uuarr;
						CHAR('m') ON_SUFFIX(3, "l", "\xc3\xbc");  // &uuml;
					CHAR_END()
				CHAR('w') ON_SUFFIX(2, "angle", "\xe2\xa6\xa7");  // &uwangle;
			CHAR_END()
		CHAR('v')
			CHAR_AT(1)
				CHAR('A') ON_SUFFIX(2, "rr", "\xe2\x87\x95");  // &vArr;
				CHAR('B')
					CHAR_AT(2)
						CHAR('a')
							CHAR_AT(3)
								CHAR('r')
									ON_LEAF_NOBR(4, "\xe2\xab\xa8");  // &vBar;
									ON_SUFFIX(4, "v", "\xe2\xab\xa9");  // &vBarv;
							CHAR_END()
					CHAR_END()
				CHAR('D') ON_SUFFIX(2, "ash", "\xe2\x8a\xa8");  // &vDash;
				CHAR('a')
					CHAR_AT(2)
						CHAR('n') ON_SUFFIX(3, "grt", "\xe2\xa6\x9c");  // &vangrt;
						CHAR('r')
							CHAR_AT(3)
								CHAR('e') ON_SUFFIX(4, "psilon", "\xcf\xb5");  // &varepsilon;
								CHAR('k') ON_SUFFIX(4, "appa", "\xcf\xb0");  // &varkappa;
								CHAR('n') ON_SUFFIX(4, "othing", "\xe2\x88\x85");  // &varnothing;
								CHAR('p')
									CHAR_AT(4)
										CHAR('h') ON_SUFFIX(5, "i", "\xcf\x95");  // &varphi;
										CHAR('i') ON_LEAF(5, "\xcf\x96");  // &varpi;
										CHAR('r') ON_SUFFIX(5, "opto", "\xe2\x88\x9d");  // &varpropto;
									CHAR_END()
								CHAR('r')
									ON_LEAF_NOBR(4, "\xe2\x86\x95");  // &varr;
									ON_SUFFIX(4, "ho", "\xcf\xb1");  // &varrho;
								CHAR('s')
									CHAR_AT(4)
										CHAR('i') ON_SUFFIX(5, "gma", "\xcf\x82");  // &varsigma;
										CHAR('u')
											CHAR_AT(5)
												CHAR('b')
													CHAR_AT(6)
														CHAR('s')
															CHAR_AT(7)
																CHAR('e')
																	CHAR_AT(8)
																		CHAR('t')
																			CHAR_AT(9)
																				CHAR('n')
																					CHAR_AT(10)
																						CHAR('e')
																							CHAR_AT(11)
																								CHAR('q')
																									ON_LEAF_NOBR(12, "\xe2\x8a\x8a\xef\xb8\x80");  // &varsubsetneq;
																									ON_SUFFIX(12, "q", "\xe2\xab\x8b\xef\xb8\x80");  // &varsubsetneqq;
																							CHAR_END()
																					CHAR_END()
																			CHAR_END()
																	CHAR_END()
															CHAR_END()
													CHAR_END()
												CHAR('p')
													CHAR_AT(6)
														CHAR('s')
															CHAR_AT(7)
																CHAR('e')
																	CHAR_AT(8)
																		CHAR('t')
																			CHAR_AT(9)
																				CHAR('n')
																					CHAR_AT(10)
																						CHAR('e')
																							CHAR_AT(11)
																								CHAR('q')
																									ON_LEAF_NOBR(12, "\xe2\x8a\x8b\xef\xb8\x80");  // &varsupsetneq;
																									ON_SUFFIX(12, "q", "\xe2\xab\x8c\xef\xb8\x80");  // &varsupsetneqq;
																							CHAR_END()
																					CHAR_END()
																			CHAR_END()
																	CHAR_END()
															CHAR_END()
													CHAR_END()
											CHAR_END()
									CHAR_END()
								CHAR('t')
									CHAR_AT(4)
										CHAR('h') ON_SUFFIX(5, "eta", "\xcf\x91");  // &vartheta;
										CHAR('r')
											CHAR_AT(5)
												CHAR('i')
													CHAR_AT(6)
														CHAR('a')
															CHAR_AT(7)
																CHAR('n')
																	CHAR_AT(8)
																		CHAR('g')
																			CHAR_AT(9)
																				CHAR('l')
																					CHAR_AT(10)
																						CHAR('e')
																							CHAR_AT(11)
																								CHAR('l') ON_SUFFIX(12, "eft", "\xe2\x8a\xb2");  // &vartriangleleft;
																								CHAR('r') ON_SUFFIX(12, "ight", "\xe2\x8a\xb3");  // &vartriangleright;
																							CHAR_END()
																					CHAR_END()
																			CHAR_END()
																	CHAR_END()
															CHAR_END()
													CHAR_END()
											CHAR_END()
									CHAR_END()
							CHAR_END()
					CHAR_END()
				CHAR('c') ON_SUFFIX(2, "y", "\xd0\xb2");  // &vcy;
				CHAR('d') ON_SUFFIX(2, "ash", "\xe2\x8a\xa2");  // &vdash;
				CHAR('e')
					CHAR_AT(2)
						CHAR('e')
							ON_LEAF_NOBR(3, "\xe2\x88\xa8");  // &vee;
							CHAR_AT(3)
								CHAR('b') ON_SUFFIX(4, "ar", "\xe2\x8a\xbb");  // &veebar;
								CHAR('e') ON_SUFFIX(4, "q", "\xe2\x89\x9a");  // &veeeq;
							CHAR_END()
						CHAR('l') ON_SUFFIX(3, "lip", "\xe2\x8b\xae");  // &vellip;
						CHAR('r')
							CHAR_AT(3)
								CHAR('b') ON_SUFFIX(4, "ar", "|");  // &verbar;
								CHAR('t') ON_LEAF(4, "|");  // &vert;
							CHAR_END()
					CHAR_END()
				CHAR('f') ON_SUFFIX(2, "r", "\xf0\x9d\x94\xb3");  // &vfr;
				CHAR('l') ON_SUFFIX(2, "tri", "\xe2\x8a\xb2");  // &vltri;
				CHAR('n')
					CHAR_AT(2)
						CHAR('s')
							CHAR_AT(3)
								CHAR('u')
									CHAR_AT(4)
										CHAR('b') ON_LEAF(5, "\xe2\x8a\x82\xe2\x83\x92");  // &vnsub;
										CHAR('p') ON_LEAF(5, "\xe2\x8a\x83\xe2\x83\x92");  // &vnsup;
									CHAR_END()
							CHAR_END()
					CHAR_END()
				CHAR('o') ON_SUFFIX(2, "pf", "\xf0\x9d\x95\xa7");  // &vopf;
				CHAR('p') ON_SUFFIX(2, "rop", "\xe2\x88\x9d");  // &vprop;
				CHAR('r') ON_SUFFIX(2, "tri", "\xe2\x8a\xb3");  // &vrtri;
				CHAR('s')
					CHAR_AT(2)
						CHAR('c') ON_SUFFIX(3, "r", "\xf0\x9d\x93\x8b");  // &vscr;
						CHAR('u')
							CHAR_AT(3)
								CHAR('b')
									CHAR_AT(4)
										CHAR('n')
											CHAR_AT(5)
												CHAR('E') ON_LEAF(6, "\xe2\xab\x8b\xef\xb8\x80");  // &vsubnE;
												CHAR('e') ON_LEAF(6, "\xe2\x8a\x8a\xef\xb8\x80");  // &vsubne;
											CHAR_END()
									CHAR_END()
								CHAR('p')
									CHAR_AT(4)
										CHAR('n')
											CHAR_AT(5)
												CHAR('E') ON_LEAF(6, "\xe2\xab\x8c\xef\xb8\x80");  // &vsupnE;
												CHAR('e') ON_LEAF(6, "\xe2\x8a\x8b\xef\xb8\x80");  // &vsupne;
											CHAR_END()
									CHAR_END()
							CHAR_END()
					CHAR_END()
				CHAR('z') ON_SUFFIX(2, "igzag", "\xe2\xa6\x9a");  // &vzigzag;
			CHAR_END()
		CHAR('w')
			CHAR_AT(1)
				CHAR('c') ON_SUFFIX(2, "irc", "\xc5\xb5");  // &wcirc;
				CHAR('e')
					CHAR_AT(2)
						CHAR('d')
							CHAR_AT(3)
								CHAR('b') ON_SUFFIX(4, "ar", "\xe2\xa9\x9f");  // &wedbar;
								CHAR('g')
									CHAR_AT(4)
										CHAR('e')
											ON_LEAF_NOBR(5, "\xe2\x88\xa7");  // &wedge;
											ON_SUFFIX(5, "q", "\xe2\x89\x99");  // &wedgeq;
									CHAR_END()
							CHAR_END()
						CHAR('i') ON_SUFFIX(3, "erp", "\xe2\x84\x98");  // &weierp;
					CHAR_END()
				CHAR('f') ON_SUFFIX(2, "r", "\xf0\x9d\x94\xb4");  // &wfr;
				CHAR('o') ON_SUFFIX(2, "pf", "\xf0\x9d\x95\xa8");  // &wopf;
				CHAR('p') ON_LEAF(2, "\xe2\x84\x98");  // &wp;
				CHAR('r')
					ON_LEAF_NOBR(2, "\xe2\x89\x80");  // &wr;
					ON_SUFFIX(2, "eath", "\xe2\x89\x80");  // &wreath;
				CHAR('s') ON_SUFFIX(2, "cr", "\xf0\x9d\x93\x8c");  // &wscr;
			CHAR_END()
		CHAR('x')
			CHAR_AT(1)
				CHAR('c')
					CHAR_AT(2)
						CHAR('a') ON_SUFFIX(3, "p", "\xe2\x8b\x82");  // &xcap;
						CHAR('i') ON_SUFFIX(3, "rc", "\xe2\x97\xaf");  // &xcirc;
						CHAR('u') ON_SUFFIX(3, "p", "\xe2\x8b\x83");  // &xcup;
					CHAR_END()
				CHAR('d') ON_SUFFIX(2, "tri", "\xe2\x96\xbd");  // &xdtri;
				CHAR('f') ON_SUFFIX(2, "r", "\xf0\x9d\x94\xb5");  // &xfr;
				CHAR('h')
					CHAR_AT(2)
						CHAR('A') ON_SUFFIX(3, "rr", "\xe2\x9f\xba");  // &xhArr;
						CHAR('a') ON_SUFFIX(3, "rr", "\xe2\x9f\xb7");  // &xharr;
					CHAR_END()
				CHAR('i') ON_LEAF(2, "\xce\xbe");  // &xi;
				CHAR('l')
					CHAR_AT(2)
						CHAR('A') ON_SUFFIX(3, "rr", "\xe2\x9f\xb8");  // &xlArr;
						CHAR('a') ON_SUFFIX(3, "rr", "\xe2\x9f\xb5");  // &xlarr;
					CHAR_END()
				CHAR('m') ON_SUFFIX(2, "ap", "\xe2\x9f\xbc");  // &xmap;
				CHAR('n') ON_SUFFIX(2, "is", "\xe2\x8b\xbb");  // &xnis;
				CHAR('o')
					CHAR_AT(2)
						CHAR('d') ON_SUFFIX(3, "ot", "\xe2\xa8\x80");  // &xodot;
						CHAR('p')
							CHAR_AT(3)
								CHAR('f') ON_LEAF(4, "\xf0\x9d\x95\xa9");  // &xopf;
								CHAR('l') ON_SUFFIX(4, "us", "\xe2\xa8\x81");  // &xoplus;
							CHAR_END()
						CHAR('t') ON_SUFFIX(3, "ime", "\xe2\xa8\x82");  // &xotime;
					CHAR_END()
				CHAR('r')
					CHAR_AT(2)
						CHAR('A') ON_SUFFIX(3, "rr", "\xe2\x9f\xb9");  // &xrArr;
						CHAR('a') ON_SUFFIX(3, "rr", "\xe2\x9f\xb6");  // &xrarr;
					CHAR_END()
				CHAR('s')
					CHAR_AT(2)
						CHAR('c') ON_SUFFIX(3, "r", "\xf0\x9d\x93\x8d");  // &xscr;
						CHAR('q') ON_SUFFIX(3, "cup", "\xe2\xa8\x86");  // &xsqcup;
					CHAR_END()
				CHAR('u')
					CHAR_AT(2)
						CHAR('p') ON_SUFFIX(3, "lus", "\xe2\xa8\x84");  // &xuplus;
						CHAR('t') ON_SUFFIX(3, "ri", "\xe2\x96\xb3");  // &xutri;
					CHAR_END()
				CHAR('v') ON_SUFFIX(2, "ee", "\xe2\x8b\x81");  // &xvee;
				CHAR('w') ON_SUFFIX(2, "edge", "\xe2\x8b\x80");  // &xwedge;
			CHAR_END()
		CHAR('y')
			CHAR_AT(1)
				CHAR('a')
					CHAR_AT(2)
						CHAR('c')
							CHAR_AT(3)
								CHAR('u') ON_SUFFIX(4, "te", "\xc3\xbd");  // &yacute;
								CHAR('y') ON_LEAF(4, "\xd1\x8f");  // &yacy;
							CHAR_END()
					CHAR_END()
				CHAR('c')
					CHAR_AT(2)
						CHAR('i') ON_SUFFIX(3, "rc", "\xc5\xb7");  // &ycirc;
						CHAR('y') ON_LEAF(3, "\xd1\x8b");  // &ycy;
					CHAR_END()
				CHAR('e') ON_SUFFIX(2, "n", "\xc2\xa5");  // &yen;
				CHAR('f') ON_SUFFIX(2, "r", "\xf0\x9d\x94\xb6");  // &yfr;
				CHAR('i') ON_SUFFIX(2, "cy", "\xd1\x97");  // &yicy;
				CHAR('o') ON_SUFFIX(2, "pf", "\xf0\x9d\x95\xaa");  // &yopf;
				CHAR('s') ON_SUFFIX(2, "cr", "\xf0\x9d\x93\x8e");  // &yscr;
				CHAR('u')
					CHAR_AT(2)
						CHAR('c') ON_SUFFIX(3, "y", "\xd1\x8e");  // &yucy;
						CHAR('m') ON_SUFFIX(3, "l", "\xc3\xbf");  // &yuml;
					CHAR_END()
			CHAR_END()
		CHAR('z')
			CHAR_AT(1)
				CHAR('a') ON_SUFFIX(2, "cute", "\xc5\xba");  // &zacute;
				CHAR('c')
					CHAR_AT(2)
						CHAR('a') ON_SUFFIX(3, "ron", "\xc5\xbe");  // &zcaron;
						CHAR('y') ON_LEAF(3, "\xd0\xb7");  // &zcy;
					CHAR_END()
				CHAR('d') ON_SUFFIX(2, "ot", "\xc5\xbc");  // &zdot;
				CHAR('e')
					CHAR_AT(2)
						CHAR('e') ON_SUFFIX(3, "trf", "\xe2\x84\xa8");  // &zeetrf;
						CHAR('t') ON_SUFFIX(3, "a", "\xce\xb6");  // &zeta;
					CHAR_END()
				CHAR('f') ON_SUFFIX(2, "r", "\xf0\x9d\x94\xb7");  // &zfr;
				CHAR('h') ON_SUFFIX(2, "cy", "\xd0\xb6");  // &zhcy;
				CHAR('i') ON_SUFFIX(2, "grarr", "\xe2\x87\x9d");  // &zigrarr;
				CHAR('o') ON_SUFFIX(2, "pf", "\xf0\x9d\x95\xab");  // &zopf;
				CHAR('s') ON_SUFFIX(2, "cr", "\xf0\x9d\x93\x8f");  // &zscr;
				CHAR('w')
					CHAR_AT(2)
						CHAR('j') ON_LEAF(3, "\xe2\x80\x8d");  // &zwj;
						CHAR('n') ON_SUFFIX(3, "j", "\xe2\x80\x8c");  // &zwnj;
					CHAR_END()
			CHAR_END()
	CHAR_END_NOBR()

	return nullptr;
}

#undef CHAR_AT
#undef CHAR
#undef CHAR_END_NOBR
#undef CHAR_END
#undef ON_SUFFIX
#undef ON_LEAF_NOBR
#undef ON_LEAF


}  // namespace tangle
// clang-format on
