/*
	Copyright (C) 2008, 2009 Andres Cabrera
	mantaraya36@gmail.com

	This file is part of CsoundQt.

	CsoundQt is free software; you can redistribute it
	and/or modify it under the terms of the GNU Lesser General Public
	License as published by the Free Software Foundation; either
	version 2.1 of the License, or (at your option) any later version.

	CsoundQt is distributed in the hope that it will be useful,
	but WITHOUT ANY WARRANTY; without even the implied warranty of
	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
	GNU Lesser General Public License for more details.

	You should have received a copy of the GNU Lesser General Public
	License along with Csound; if not, write to the Free Software
	Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA
	02111-1307 USA
*/

#include "highlighter.h"

#include <QDebug>

TextBlockData::TextBlockData()
{
	// Nothing to do
}

QVector<ParenthesisInfo *> TextBlockData::parentheses()
{
	return m_parentheses;
}


void TextBlockData::insert(ParenthesisInfo *info)
{
	int i = 0;
	while (i < m_parentheses.size() &&
		info->position > m_parentheses.at(i)->position)
		++i;

	m_parentheses.insert(i, info);
}

Highlighter::Highlighter(QTextDocument *parent)
	: QSyntaxHighlighter(parent)
{
	commentStartExpression = QRegExp("/\\*");
	commentEndExpression = QRegExp("\\*/");

	//  b64encStartExpression = QRegExp("<CsFileB .*>");
	//  b64encEndExpression = QRegExp("<CsFileB>");
	colorVariables = true;
	m_mode = 0; // default to Csound mode

	// For Csound
	csdtagFormat.setForeground(QColor("brown"));
	csdtagFormat.setFontWeight(QFont::Bold);
	tagPatterns << "<CsoundSynthesizer>" << "</CsoundSynthesizer>"
				<< "<CsInstruments>" << "</CsInstruments>"
				<< "<CsOptions>" << "</CsOptions>"
				<< "<CsScore>" << "</CsScore>"
				<< "<CsVersion>" << "</CsVersion>"
				<< "<MacOptions>" << "</MacOptions>"
				<< "<MacGUI>" << "</MacGUI>"
				<< "<csLADSPA>" << "</csLADSPA>"
                << "<Cabbage>" << "</Cabbage>"
				<< "<CsHtml5>" << "</CsHtml5>"
				<< "<CsFileB>" << "</CsFileB>"    ;
	instFormat.setForeground(QColor("purple"));
	instFormat.setFontWeight(QFont::Bold);
	instPatterns << "instr" << "endin" << "opcode" << "endop";
    headerPatterns << "sr" << "kr" << "ksmps" << "nchnls" << "0dbfs" << "A4";
	keywordPatterns << "do" << "od";
	csdtagFormat.setForeground(QColor("brown"));
	csdtagFormat.setFontWeight(QFont::Bold);
	opcodeFormat.setForeground(QColor("blue"));
	opcodeFormat.setFontWeight(QFont::Bold);

	// For Python
	keywords << "and" << "or" << "not" << "is";
	keywords << "global" << "with" << "from" << "import" << "as";
	keywords << "if" << "else" << "elif";
	keywords << "print" << "class" << "del" << "exec";
	keywords << "for" << "in" << "while" << "continue" << "pass" << "break";
	keywords << "def" << "return" << "lambda";
	keywords << "yield" << "assert" << "try" << "except" << "finally" << "raise";
	keywords << "True" << "False" << "None";

	// for html
	htmlKeywords << "<\\ba\\b" << "<\\babbr\\b" << "<\\bacronym\\b" << "<\\baddress\\b" << "<\\bapplet\\b"
				<< "<\\barea\\b" << "<\\barticle\\b" << "<\\baside\\b" << "<\\baudio\\b" << "<\\bb\\b"
				<< "<\\bbase\\b" << "<\\bbasefont\\b" << "<\\bbdi\\b" << "<\\bbdo\\b" << "<\\bbgsound\\b"
				<< "<\\bblockquote\\b" << "<\\bbig\\b" << "<\\bbody\\b" << "<\\bblink\\b" << "<\\bbr\\b"
				<< "<\\bbutton\\b" << "<\\bcanvas\\b" << "<\\bcaption\\b" << "<\\bcenter\\b" << "<\\bcite\\b"
				<< "<\\bcode\\b" << "<\\bcol\\b" << "<\\bcolgroup\\b" << "<\\bcommand\\b" << "<\\bcomment\\b"
				<< "<\\bdata\\b" << "<\\bdatalist\\b" << "<\\bdd\\b" << "<\\bdel\\b" << "<\\bdetails\\b"
				<< "<\\bdfn\\b" << "<\\bdialog\\b" << "<\\bdir\\b" << "<\\bdiv\\b" << "<\\bdl\\b"
				<< "<\\bdt\\b" << "<\\bem\\b" << "<\\bembed\\b" << "<\\bfieldset\\b" << "<\\bfigcaption\\b"
				<< "<\\bfigure\\b" << "<\\bfont\\b" << "<\\bfooter\\b" << "<\\bform\\b" << "<\\bframe\\b"
				<< "<\\bframeset\\b" << "<\\bh1\\b" << "<\\bh2\\b" << "<\\bh3\\b" << "<\\bh4\\b"
				<< "<\\bh5\\b" << "<\\bh6\\b" << "<\\bhead\\b" << "<\\bheader\\b" << "<\\bhgroup\\b"
				<< "<\\bhr\\b" << "<\\bhtml\\b" << "<\\bi\\b" << "<\\biframe\\b" << "<\\bimg\\b"
				<< "<\\binput\\b" << "<\\bins\\b" << "<\\bisindex\\b" << "<\\bkbd\\b" << "<\\bkeygen\\b"
				<< "<\\blabel\\b" << "<\\blegend\\b" << "<\\bli\\b" << "<\\blink\\b" << "<\\blisting\\b"
				<< "<\\bmain\\b" << "<\\bmap\\b" << "<\\bmarquee\\b" << "<\\bmark\\b" << "<\\bmenu\\b"
				<< "<\\bamenuitem\\b" << "<\\bmeta\\b" << "<\\bmeter\\b" << "<\\bmulticol\\b" << "<\\bnav\\b"
				<< "<\\bnobr\\b" << "<\\bnoembed\\b" << "<\\bnoindex\\b" << "<\\bnoframes\\b" << "<\\bnoscript\\b"
				<< "<\\bobject\\b" << "<\\bol\\b" << "<\\boptgroup\\b" << "<\\boption\\b" << "<\\boutput\\b"
				<< "<\\bp\\b" << "<\\bparam\\b" << "<\\bpicture\\b" << "<\\bplaintext\\b" << "<\\bpre\\b"
				<< "<\\bprogress\\b" << "<\\bq\\b" << "<\\brp\\b" << "<\\brt\\b" << "<\\brtc\\b" << "<\\bruby\\b"
				<< "<\\bs\\b" << "<\\bsamp\\b" << "<\\bscript\\b" << "<\\bsection\\b" << "<\\bselect\\b"
				<< "<\\bsmall\\b" << "<\\bsource\\b" << "<\\bspacer\\b" << "<\\bspan\\b" << "<\\bstrike\\b"
				<< "<\\bstrong\\b" << "<\\bstyle\\b" << "<\\bsub\\b" << "<\\bsummary\\b" << "<\\bsup\\b"
				<< "<\\btable\\b" << "<\\btbody\\b" << "<\\btd\\b" << "<\\btemplate\\b" << "<\\btextarea\\b"
				<< "<\\btfoot\\b" << "<\\bth\\b" << "<\\bthead\\b" << "<\\btime\\b" << "<\\btitle\\b"
				<< "<\\btr\\b" << "<\\btrack\\b" << "<\\btt\\b" << "<\\bu\\b" << "<\\bul\\b" << "<\\bvar\\b"
				<< "<\\bvideo\\b" << "<\\bwbr\\b" << "<\\bxmp\\b";




	htmlKeywords << "<!\\bDOCTYPE\\b" << "</\\ba\\b" << "</\\babbr\\b" << "</\\bacronym\\b" << "</\\baddress\\b" << "</\\bapplet\\b"
				<< "</\\barea\\b" << "</\\barticle\\b" << "</\\baside\\b" << "</\\baudio\\b" << "</\\bb\\b"
				<< "</\\bbase\\b" << "</\\bbasefont\\b" << "</\\bbdi\\b" << "</\\bbdo\\b" << "</\\bbgsound\\b"
				<< "</\\bblockquote\\b" << "</\\bbig\\b" << "</\\bbody\\b" << "</\\bblink\\b" << "</\\bbr\\b"
				<< "</\\bbutton\\b" << "</\\bcanvas\\b" << "</\\bcaption\\b" << "</\\bcenter\\b" << "</\\bcite\\b"
				<< "</\\bcode\\b" << "</\\bcol\\b" << "</\\bcolgroup\\b" << "</\\bcommand\\b" << "</\\bcomment\\b"
				<< "</\\bdata\\b" << "</\\bdatalist\\b" << "</\\bdd\\b" << "</\\bdel\\b" << "</\\bdetails\\b"
				<< "</\\bdfn\\b" << "</\\bdialog\\b" << "</\\bdir\\b" << "</\\bdiv\\b" << "</\\bdl\\b"
				<< "</\\bdt\\b" << "</\\bem\\b" << "</\\bembed\\b" << "</\\bfieldset\\b" << "</\\bfigcaption\\b"
				<< "</\\bfigure\\b" << "</\\bfont\\b" << "</\\bfooter\\b" << "</\\bform\\b" << "</\\bframe\\b"
				<< "</\\bframeset\\b" << "</\\bh1\\b" << "</\\bh2\\b" << "</\\bh3\\b" << "</\\bh4\\b"
				<< "</\\bh5\\b" << "</\\bh6\\b" << "</\\bhead\\b" << "</\\bheader\\b" << "</\\bhgroup\\b"
				<< "</\\bhr\\b" << "</\\bhtml\\b" << "</\\bi\\b" << "</\\biframe\\b" << "</\\bimg\\b"
				<< "</\\binput\\b" << "</\\bins\\b" << "</\\bisindex\\b" << "</\\bkbd\\b" << "</\\bkeygen\\b"
				<< "</\\blabel\\b" << "</\\blegend\\b" << "</\\bli\\b" << "</\\blink\\b" << "</\\blisting\\b"
				<< "</\\bmain\\b" << "</\\bmap\\b" << "</\\bmarquee\\b" << "</\\bmark\\b" << "</\\bmenu\\b"
				<< "</\\bamenuitem\\b" << "</\\bmeta\\b" << "</\\bmeter\\b" << "</\\bmulticol\\b" << "</\\bnav\\b"
				<< "</\\bnobr\\b" << "</\\bnoembed\\b" << "</\\bnoindex\\b" << "</\\bnoframes\\b" << "</\\bnoscript\\b"
				<< "</\\bobject\\b" << "</\\bol\\b" << "</\\boptgroup\\b" << "</\\boption\\b" << "</\\boutput\\b"
				<< "</\\bp\\b" << "</\\bparam\\b" << "</\\bpicture\\b" << "</\\bplaintext\\b" << "</\\bpre\\b"
				<< "</\\bprogress\\b" << "</\\bq\\b" << "</\\brp\\b" << "</\\brt\\b" << "</\\brtc\\b" << "</\\bruby\\b"
				<< "</\\bs\\b" << "</\\bsamp\\b" << "</\\bscript\\b" << "</\\bsection\\b" << "</\\bselect\\b"
				<< "</\\bsmall\\b" << "</\\bsource\\b" << "</\\bspacer\\b" << "</\\bspan\\b" << "</\\bstrike\\b"
				<< "</\\bstrong\\b" << "</\\bstyle\\b" << "</\\bsub\\b" << "</\\bsummary\\b" << "</\\bsup\\b"
				<< "</\\btable\\b" << "</\\btbody\\b" << "</\\btd\\b" << "</\\btemplate\\b" << "</\\btextarea\\b"
				<< "</\\btfoot\\b" << "</\\bth\\b" << "</\\bthead\\b" << "</\\btime\\b" << "</\\btitle\\b"
				<< "</\\btr\\b" << "</\\btrack\\b" << "</\\btt\\b" << "</\\bu\\b" << "</\\bul\\b" << "</\\bvar\\b"
				<< "</\\bvideo\\b" << "</\\bwbr\\b" << "</\\bxmp\\b";

	javascriptKeywords << "function" << "var" << "if" << "===" << "console.log"  << "console.warn";

	keywordFormat.setForeground(QColor("blue"));
	keywordFormat.setFontWeight(QFont::Bold);

	singleLineCommentFormat.setForeground(QColor("green"));
	singleLineCommentFormat.setFontItalic(true);

	macroDefineFormat.setForeground(QColor("green"));
	macroDefineFormat.setFontWeight(QFont::Bold);

	pfieldFormat.setFontWeight(QFont::Bold);

	irateFormat.setForeground(QColor("darkCyan"));
	krateFormat.setForeground(QColor("darkCyan"));
	arateFormat.setForeground(QColor("darkCyan"));
	arateFormat.setFontWeight(QFont::Bold);

	girateFormat.setForeground(QColor("darkCyan"));
	girateFormat.setFontItalic(true);
	gkrateFormat.setForeground(QColor("darkCyan"));
	gkrateFormat.setFontItalic(true);
	garateFormat.setForeground(QColor("darkCyan"));
	garateFormat.setFontWeight(QFont::Bold);
	garateFormat.setFontItalic(true);

	stringVarFormat.setForeground(QColor(Qt::darkYellow));
	stringVarFormat.setFontWeight(QFont::Bold);
	gstringVarFormat.setForeground(QColor(Qt::darkYellow));
	gstringVarFormat.setFontWeight(QFont::Bold);
	gstringVarFormat.setFontItalic(true);

	fsigFormat.setForeground(QColor(Qt::gray));
	fsigFormat.setFontWeight(QFont::Bold);
	gfsigFormat.setForeground(QColor(Qt::gray));
	gfsigFormat.setFontItalic(true);
	gfsigFormat.setFontWeight(QFont::Bold);

	// for html
//	jsKeywordFormat.setForeground(QColor(Qt::black));
//	jsKeywordFormat.setFontWeight(QFont::Bold);
	jsKeywordFormat = keywordFormat;

	htmlTagFormat.setForeground(QColor(Qt::cyan));
	htmlTagFormat.setFontWeight(QFont::Bold);
	//setFormatFor(Tag, csdtagFormat);

//	QTextCharFormat commentFormat;
//	commentFormat.setForeground(QColor(128, 10, 74));
//	commentFormat.setFontItalic(true);
//	setFormatFor(Comment, commentFormat);
//	setFormatFor(Comment, singleLineCommentFormat);


}


Highlighter::~Highlighter()
{
}

void Highlighter::setOpcodeNameList(QStringList list)
{
	m_opcodeList = list;
	//   setFirstRules();
	setLastRules();
}

void Highlighter::setMode(int mode)
{
	m_mode = mode;
}


void Highlighter::setColorVariables(bool color)
{
	colorVariables = color;

	highlightingRules.clear();
	setLastRules();
}

void Highlighter::highlightBlock(const QString &text)
{
	switch (m_mode) {
	case 0:  // Csound mode
		highlightCsoundBlock(text);
		break;
	case 1:  // Python mode
		highlightPythonBlock(text);
		break;
	case 2:  // Xml mode
		highlightXmlBlock(text);
		break;
	case 3:  // Orc
		highlightCsoundBlock(text);
		break;
    case 4:  // Sco
		highlightCsoundBlock(text);
		break;
    case 5:  // Inc
        highlightCsoundBlock(text); // maybe anything not python or xml should be higlighter as csound?
        break;
	case 6:  // Html
		highlightHtmlBlock(text);
		break;
	}
	// for parenthesis
	TextBlockData *data = new TextBlockData;

	int leftPos = text.indexOf('(');
	while (leftPos != -1) {
		ParenthesisInfo *info = new ParenthesisInfo;
		info->character = '(';
		info->position = leftPos;

		data->insert(info);
		leftPos = text.indexOf('(', leftPos + 1);
	}

	int rightPos = text.indexOf(')');
	while (rightPos != -1) {
		ParenthesisInfo *info = new ParenthesisInfo;
		info->character = ')';
		info->position = rightPos;

		data->insert(info);

		rightPos = text.indexOf(')', rightPos +1);
	}

	setCurrentBlockUserData(data);
}

void Highlighter::highlightCsoundBlock(const QString &text)
{
	// text is processed one line at a time
	//qDebug("Text---------------------: %s", text.toStdString().c_str());

	int commentIndex = text.indexOf("//"); // try both comment markings
	if (commentIndex < 0) {
		commentIndex = text.indexOf(';');
	}
	if (commentIndex >= 0) {
		setFormat(commentIndex, text.size() - commentIndex, singleLineCommentFormat);
//		return;
	}
	else {
		commentIndex = text.size() + 1;
	}
	int macroIndex = text.indexOf('#');
	if (macroIndex >= 0 && macroIndex < commentIndex) {
		setFormat(macroIndex, text.size() - macroIndex, macroDefineFormat);
		commentIndex = macroIndex;
	}
	QRegExp expression("\\b+[\\w:]+\\b"); // how to find, if macro name, starts with '$'?
	int index = text.indexOf(expression, 0);
	int length = expression.matchedLength();

	while (index >= 0 && index < commentIndex) {
		int wordStart = index;
		int wordEnd = wordStart + length;
		if (index>0 && text.at(index-1)=='$') { // check if macro name - replacement for regexp solution which I could not find
			wordStart--;
			length++;
			//qDebug()<<"Found macro ";
			setFormat(wordStart, wordEnd - wordStart, macroDefineFormat);
		}
		wordEnd = (wordEnd > 0 ? wordEnd : text.size());
		QString word = text.mid(wordStart, length);
//		qDebug() << "word: " << word;
		if (word.indexOf(QRegExp("p[\\d]+\\b")) != -1) {
			setFormat(wordStart, wordEnd - wordStart, pfieldFormat);
		}
		if (instPatterns.contains(word)) {
			setFormat(wordStart, wordEnd - wordStart, instFormat);
			break; // was: return. FOr any case, to go through lines after while loop
		}
		else if (tagPatterns.contains("<" + word + ">") && wordStart > 0) {
			setFormat(wordStart - (text[wordStart - 1] == '/' ?  2 : 1), wordEnd - wordStart + (text[wordStart - 1] == '/' ?  3 : 2), csdtagFormat);
		}
		else if (headerPatterns.contains(word)) {
			setFormat(wordStart, wordEnd - wordStart, csdtagFormat);
		}
		else if (keywordPatterns.contains(word)) {
			setFormat(wordStart, wordEnd - wordStart, opcodeFormat);
		}
		else if (word.contains(":")) {
			QStringList parts = word.split(":");
			if (parts.size() == 2) {
				if (findOpcode(parts[0]) >= 0) {
					setFormat(wordStart, wordEnd - wordStart, opcodeFormat);
				}
			}
		}
		else if (findOpcode(word) >= 0) {
			setFormat(wordStart, wordEnd - wordStart, opcodeFormat);
		}
		else if (word.startsWith('a') && colorVariables) {
			setFormat(wordStart, wordEnd - wordStart, arateFormat);
		}
		else if ((word.startsWith('k') || word.startsWith('i')) && colorVariables) {
			setFormat(wordStart, wordEnd - wordStart, krateFormat);
		}
		else if (word.startsWith("ga")  && colorVariables) {
			setFormat(wordStart, wordEnd - wordStart, garateFormat);
		}
		else if ((word.startsWith("gk") || word.startsWith("gi")) && colorVariables) {
			setFormat(wordStart, wordEnd - wordStart, gkrateFormat);
		}
		else if (word.startsWith("S")  && colorVariables) {
			setFormat(wordStart, wordEnd - wordStart, stringVarFormat);
		}
		else if (word.startsWith("gS")  && colorVariables) {
			setFormat(wordStart, wordEnd - wordStart, gstringVarFormat);
		}
		else if (word.startsWith("f") && colorVariables) {
			setFormat(wordStart, wordEnd - wordStart, fsigFormat);
		}
		else if (word.startsWith("gf") && colorVariables) {
			setFormat(wordStart, wordEnd - wordStart, gfsigFormat);
		}
		index = text.indexOf(expression, wordEnd);
		length = expression.matchedLength();
	}
	//last rules
	for (int i = 0; i < lastHighlightingRules.size(); i++) {
		QRegExp expression(lastHighlightingRules[i].pattern);
		//     QString temp = rule.pattern.pattern();
		int index = text.indexOf(expression);
		while (index >= 0 && index < commentIndex) {
			int length = expression.matchedLength();
			setFormat(index, length, lastHighlightingRules[i].format);
			index = text.indexOf(expression, index + length);
		}
	}

	setCurrentBlockState(0);

	int startIndex = 0;
	if (previousBlockState() != 1) {
		startIndex = text.indexOf(commentStartExpression);
	}

	while (startIndex >= 0 && startIndex < commentIndex) {
		int endIndex = text.indexOf(commentEndExpression, startIndex);
		if (format(startIndex) == quotationFormat) {
			startIndex = text.indexOf(commentStartExpression,
									  startIndex + 1);
			continue;
		}
		int commentLength;
		if (endIndex == -1) {
			setCurrentBlockState(1);
			commentLength = text.length() - startIndex;
		} else {
			commentLength = endIndex - startIndex
					+ commentEndExpression.matchedLength();
		}
		setFormat(startIndex, commentLength, multiLineCommentFormat);
		startIndex = text.indexOf(commentStartExpression,
								  startIndex + commentLength);
	}
}


void Highlighter::highlightPythonBlock(const QString &text)
{
	QRegExp expression("\\b+\\w\\b+");
	int index = text.indexOf(expression, 0);
	for (int i = 0; i < keywords.size(); i++) {
		QRegExp expression("\\b+" + keywords[i] + "\\b+");
		int index = text.indexOf(expression);
		while (index >= 0) {
			int length = expression.matchedLength();
			setFormat(index, length, keywordFormat);
			index = text.indexOf(expression, index + length);
		}
	}
	QRegExp strings( QRegExp("\"[^\"]*\""));
	index = text.indexOf(strings);
	while (index >= 0) {
		int length = strings.matchedLength();
		setFormat(index, length, quotationFormat);
		index = text.indexOf(strings, index + length);
	}
	strings = QRegExp("'[^'']*'");
	index = text.indexOf(strings);
	while (index >= 0) {
		int length = strings.matchedLength();
		setFormat(index, length, quotationFormat);
		index = text.indexOf(strings, index + length);
	}
	QRegExp expComment("#.*");
	index = text.indexOf(expComment);
	while (index >= 0) {
		int length = expComment.matchedLength();
		setFormat(index, length, singleLineCommentFormat);
		index = text.indexOf(expComment, index + length);
	}
}

void Highlighter::highlightXmlBlock(const QString &/*text*/)
{
}

// for html

void Highlighter::highlightHtmlBlock(const QString &text)
{
	QRegExp expression("\\b+\\w\\b+");
	int index = text.indexOf(expression, 0);
	for (int i = 0; i < htmlKeywords.size(); i++) {
		QRegExp expression(htmlKeywords[i]);//expression("\\b+" + htmlKeywords[i] + "\\b+");
		int index = text.indexOf(expression);
		while (index >= 0) {
			int length = expression.matchedLength();
			setFormat(index, length, keywordFormat);
			index = text.indexOf(expression, index + length);
		}
	}

	for (int i=0; i<javascriptKeywords.size(); i++) {
		QRegExp expression("\\b+" + javascriptKeywords[i] + "\\b+");
		int index = text.indexOf(expression);
		while (index >= 0) {
			int length = expression.matchedLength();
			setFormat(index, length, jsKeywordFormat); // TODO javascriptformat
			index = text.indexOf(expression, index + length);
		}
	}

	QRegExp endTag( QRegExp(">$"));
	index = text.indexOf(endTag);
	while (index >= 0) {
		int length = endTag.matchedLength();
		setFormat(index, length, keywordFormat);
		index = text.indexOf(endTag, index + length);
	}


	QRegExp strings( QRegExp("\"[^\"]*\""));
	index = text.indexOf(strings);
	while (index >= 0) {
		int length = strings.matchedLength();
		setFormat(index, length, quotationFormat);
		index = text.indexOf(strings, index + length);
	}
	strings = QRegExp("'[^'']*'");
	index = text.indexOf(strings);
	while (index >= 0) {
		int length = strings.matchedLength();
		setFormat(index, length, quotationFormat);
		index = text.indexOf(strings, index + length);
	}
	int commentIndex = -1;
	QRegExp expComment("//.*"); // TODO: avaoid https://
	index = text.indexOf(expComment);
	if (index>0 ) {
		if (text.at(index-1)!=':') { // clumsy way to avoid addresses like https://
			while (index >= 0) { // did not manage to do it with regular expression
				int length = expComment.matchedLength();
				setFormat(index, length, singleLineCommentFormat);
				index = text.indexOf(expComment, index + length);
			}
			commentIndex = index; // better do other way
		}
	}

//	if (commentIndex >= 0) {
//		setFormat(commentIndex, text.size() - commentIndex, singleLineCommentFormat);
////		return;
//	}
	if (commentIndex < 0) {
		commentIndex = text.size() + 1;
	}

	// multiline
	setCurrentBlockState(0);
	QRegExp htmlCommentStartExpression = QRegExp("<!--");
	QRegExp htmlCommentEndExpression = QRegExp("-->");


	int startIndex = 0;
	if (previousBlockState() != 1) {
		startIndex = text.indexOf(htmlCommentStartExpression);
	}

	while (startIndex >= 0 && startIndex < commentIndex) {
		int endIndex = text.indexOf(htmlCommentEndExpression, startIndex);
		if (format(startIndex) == quotationFormat) {
			startIndex = text.indexOf(htmlCommentStartExpression,
									  startIndex + 1);
			continue;
		}
		int commentLength;
		if (endIndex == -1) {
			setCurrentBlockState(1);
			commentLength = text.length() - startIndex;
		} else {
			commentLength = endIndex - startIndex
					+ htmlCommentEndExpression.matchedLength();
		}
		setFormat(startIndex, commentLength, multiLineCommentFormat);
		startIndex = text.indexOf(htmlCommentStartExpression,
								  startIndex + commentLength);
	}


}

// void Highlighter::setFirstRules()
// {
//   highlightingRules.clear();
// }

void Highlighter::setLastRules()
{
	HighlightingRule rule;

	labelFormat.setForeground(QColor(205,92,92));
	labelFormat.setFontWeight(QFont::Bold);
	rule.pattern = QRegExp("^\\s*\\w+:\\s*");
	rule.format = labelFormat;
	lastHighlightingRules.append(rule);

	quotationFormat.setForeground(Qt::red);
//	rule.pattern = QRegExp("\".*\"");
	rule.pattern = QRegExp("\"[^\"]*\"");
	rule.format = quotationFormat;
	lastHighlightingRules.append(rule);
	rule.pattern = QRegExp("\\{\\{.*");
	rule.format = quotationFormat;
	lastHighlightingRules.append(rule);
	rule.pattern = QRegExp(".*\\}\\}");
	rule.format = quotationFormat;
	lastHighlightingRules.append(rule);

	multiLineCommentFormat.setForeground(QColor("green"));
}

int Highlighter::findOpcode(QString opcodeName, int start, int end)
{
	//   fprintf(stderr, "%i - %i\n", start, end);
	Q_ASSERT(m_opcodeList.size() > 0);
	if (end == -1) {
		end = m_opcodeList.size() -1;
	}
	int pos = ((end - start)/2) + start;
	if (opcodeName == m_opcodeList[pos])
		return pos;
	else if (start == end)
		return -1;
	else if (opcodeName < m_opcodeList[pos])
		return findOpcode(opcodeName, start, pos);
	else if (opcodeName > m_opcodeList[pos])
		return findOpcode(opcodeName, pos + 1, end);
	return -1;
}
