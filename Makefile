CXX = g++
CXXFLAGS = -std=c++14 -Wall -MMD -g
EXEC = a.out 
OBJECTS = main.o model.o view.o controller.o action.o vmstate/filestate.o vmstate/clipboard.o vmstate/history.o vmstate/editorstate.o vmstate/filecontent/filecontent.o vmstate/filecontent/reversegapbuffer.o view/fileview.o view/syntaxparser.o view/colorer.o view/commandbarview.o control/ckeyboard.o control/inputparser.o control/binding.o actions/normal/movement.o actions/normal/quickmove.o actions/normal/normalswitch.o actions/normal/changetext.o actions/normal/undoredo.o actions/normal/copypaste.o actions/normal/search.o actions/normal/normalmisc.o actions/insert/insertion.o actions/insert/insertswitch.o actions/replace/replacement.o actions/replace/replaceswitch.o actions/command/command.o actions/command/commandswitch.o
DEPENDS = ${OBJECTS:.o=.d}

${EXEC}: ${OBJECTS}
	${CXX} ${OBJECTS} -o ${EXEC} -lncurses

-include ${DEPENDS}

.PHONY: clean

clean:
	rm ${OBJECTS} ${DEPENDS} ${EXEC}
