#include "CPerceptron.h"

CPerceptron::CPerceptron(const QList<int>& nbInputs, int nbNeurone, bool useBiais) {
    QList<CNeurone *> layer;
    int i;
    this->useBiais = useBiais;

    for(i=0;i<nbNeurone;i++) {
        CNeurone *n = new CNeurone(nbInputs.at(i), useBiais);
        layer.append(n);
    }

    layers.append(layer);
}

CPerceptron::~CPerceptron(void) {
    int i;

    for(i=0;i<layers.size();i++) {
        int j;
        QList<CNeurone *> layer = layers.at(i);

        for(j=0;j<layer.size();j++) {
            delete layer.at(j);
        }

        layer.clear();
    }
    layers.clear();
}

void CPerceptron::init(void) {
    int i;

    for(i=0;i<layers.size();i++) {
        int j;

        for(j=0;j<layers.at(i).size();j++) {
            layers.at(i).at(j)->init();
        }
    }
}

QList<double> CPerceptron::eval(const QList<QList<double> >& inputs) {
    QList<double> resultat;
    QList<double> in;
    int i, j;

    for(i=0;i<layers.at(0).size();i++) {
        CNeurone *n = layers.at(0).at(i);
        resultat << n->eval(inputs.at(i));
    }

    for(i=1;i<layers.size();i++) {
        in.clear();
        in << resultat;
        resultat.clear();

        for(j=0;j<layers.at(i).size();j++) {
            CNeurone *n = layers.at(i).at(j);

            resultat << n->eval(in);
        }
    }

    return resultat;
}

void CPerceptron::addLayer(int nbNeurone) {
    int nbInput = layers.last().size();
    int i;
    QList<CNeurone *> layer;

    for(i=0;i<nbNeurone;i++) {
        CNeurone *n = new CNeurone(nbInput, useBiais);
        layer.append(n);
    }

    layers.append(layer);
}

void CPerceptron::from(CPerceptron * p1, CPerceptron * p2) {
    int i;

    for(i=0;i<layers.size();i++) {
        int j;

        for(j=0;j<layers.at(i).size();j++) {
            CNeurone *n = layers.at(i).at(j);
            CNeurone *n1 = p1->layers.at(i).at(j);
            CNeurone *n2 = p2->layers.at(i).at(j);
            int seuil = rand() % n->getNbGene();

            n->from(n1, n2, seuil);
            if(rand() % 100 > 80) {
                n->mute(rand() % n->getNbGene());
            }
        }
    }
}

int CPerceptron::diff(CPerceptron * other) const {
    int i;
    int diff = 0;

    for(i=0;i<layers.size();i++) {
        int j;

        for(j=0;j<layers.at(i).size();j++) {
            CNeurone *n = layers.at(i).at(j);
            CNeurone *no = other->layers.at(i).at(j);
            diff += n->diff(no);
        }
    }

    return diff;
}

QDataStream& operator<<(QDataStream& out, const CPerceptron& perceptron) {
    int i;

    out << (qint32)perceptron.layers.size();
    out << perceptron.useBiais;
    out << (qint32)perceptron.layers.at(0).size();
    for(i=0;i<perceptron.layers.at(0).size();i++) {
        out << perceptron.layers.at(0).at(i);
    }

    for(i=1;i<perceptron.layers.size();i++) {
        int j;

        out << (qint32)perceptron.layers.at(i).size();
        for(j=0;j<perceptron.layers.at(i).size();j++) {
            out << perceptron.layers.at(i).at(j);
        }
    }

    return out;
}
