HTLATEX=htlatex
LATEX2HTML=latex2html
PDFLATEX=pdflatex -interaction nonstopmode
MAKEINDEX=makeindex
PERL=perl
THUMBPDF=thumbpdf
VIEWPDF=acroread

USER_MANUAL_SOURCES = user_manual.tex \
	$(wildcard include/*.tex) \
	../../common/images/wallpaper1_640x480_2.jpg \

all: doc

doc: user_manual.pdf 

user_manual.pdf: $(USER_MANUAL_SOURCES)
	$(PDFLATEX) $<
	$(MAKEINDEX) user_manual.idx
	$(PDFLATEX) $<

clean:
	-rm -f user_manual.pdf
	-rm -f user_manual.idx
	-rm -f user_manual.ilg
	-rm -f user_manual.ind
	-rm -f *.aux *.dvi *.log *.out *.toc *.tpt
	-rm -f include/*.aux

view: user_manual.pdf
	$(VIEWPDF) $<

.DELETE_ON_ERROR:

