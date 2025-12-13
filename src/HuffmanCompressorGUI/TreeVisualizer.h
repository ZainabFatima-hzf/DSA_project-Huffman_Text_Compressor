// TreeVisualizer.h
#pragma once

#include <QDialog>
#include <QPoint>
#include <QPair>
#include <QVector>

// Include the real HuffmanNode header
#include "../src/models/HuffmanNode.h"

// Forward declarations
class QPaintEvent;
class QPainter;

class TreeVisualizer : public QDialog
{
    Q_OBJECT

public:
    // Uses the real HuffmanNode* type
    explicit TreeVisualizer(HuffmanNode* root, QWidget *parent = nullptr);
    ~TreeVisualizer();

private:
    HuffmanNode* treeRoot;

    // Inner widget for drawing the tree
    class TreeDrawingWidget : public QWidget {
    public:
        TreeDrawingWidget(HuffmanNode* root, QWidget *parent = nullptr);
        QSize sizeHint() const override;

    protected:
        void paintEvent(QPaintEvent *event) override;

    private:
        HuffmanNode* treeRoot;
        QVector<QPair<HuffmanNode*, QPoint>> nodePositions;

        void calculateNodePositions(HuffmanNode* node, int x, int y, int h_gap);
        void drawTree(QPainter& painter, HuffmanNode* node, const QPoint& parentPos, bool isLeftChild);
        QPoint getPositionForNode(HuffmanNode* targetNode) const;
        int calculateTreeWidth(HuffmanNode* node);
    };

    TreeDrawingWidget* drawingWidget;
};
