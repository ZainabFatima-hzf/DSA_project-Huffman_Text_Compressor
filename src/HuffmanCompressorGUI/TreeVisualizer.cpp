#include "TreeVisualizer.h"
#include <QPainter>
#include <QPen>
#include <QBrush>
#include <QFont>
#include <QDebug>
#include <QPaintEvent>
#include <QVBoxLayout>
#include <QScrollArea>
#include <QLabel>
#include <QPushButton>
#include <algorithm>
#include <climits>

// --- Constants for Drawing Layout ---
const int NODE_RADIUS = 25;
const int V_GAP = 120;
const int PADDING = 60;
// ------------------------------------

// -------------------------------------------------------------
// TreeDrawingWidget (Inner class implementation)
// -------------------------------------------------------------

TreeVisualizer::TreeDrawingWidget::TreeDrawingWidget(HuffmanNode* root, QWidget *parent)
    : QWidget(parent), treeRoot(root)
{
    setAutoFillBackground(true);
    setPalette(QPalette(Qt::white));

    if (treeRoot) {
        // Calculate positions
        int initialWidth = calculateTreeWidth(treeRoot) * 100;
        calculateNodePositions(treeRoot, initialWidth / 2, NODE_RADIUS + PADDING, initialWidth / 4);

        // Calculate required size
        int minX = INT_MAX, maxX = INT_MIN;
        int minY = INT_MAX, maxY = INT_MIN;

        for (const auto& pair : nodePositions) {
            const QPoint& pos = pair.second;
            minX = std::min(minX, pos.x());
            maxX = std::max(maxX, pos.x());
            minY = std::min(minY, pos.y());
            maxY = std::max(maxY, pos.y());
        }

        int width = maxX - minX + 4 * NODE_RADIUS + 2 * PADDING;
        int height = maxY + 4 * NODE_RADIUS + 2 * PADDING;

        // Adjust positions to be positive
        for (auto& pair : nodePositions) {
            pair.second.setX(pair.second.x() - minX + NODE_RADIUS + PADDING);
            pair.second.setY(pair.second.y() + NODE_RADIUS + PADDING);
        }

        setMinimumSize(width, height);
        qDebug() << "Tree drawing widget size:" << width << "x" << height;
    } else {
        setMinimumSize(400, 300);
    }
}

QSize TreeVisualizer::TreeDrawingWidget::sizeHint() const
{
    return minimumSize();
}

int TreeVisualizer::TreeDrawingWidget::calculateTreeWidth(HuffmanNode* node)
{
    if (!node) return 0;
    if (!node->left && !node->right) return 1;
    return calculateTreeWidth(node->left) + calculateTreeWidth(node->right);
}

void TreeVisualizer::TreeDrawingWidget::calculateNodePositions(HuffmanNode* node, int x, int y, int h_gap)
{
    if (!node) return;

    // Store position
    nodePositions.append({node, QPoint(x, y)});

    // Reduce gap for deeper levels but not too much
    int new_h_gap = h_gap * 0.75;
    if (new_h_gap < 80) new_h_gap = 80;

    // Position children
    if (node->left) {
        calculateNodePositions(node->left, x - new_h_gap, y + V_GAP, new_h_gap);
    }
    if (node->right) {
        calculateNodePositions(node->right, x + new_h_gap, y + V_GAP, new_h_gap);
    }
}

QPoint TreeVisualizer::TreeDrawingWidget::getPositionForNode(HuffmanNode* targetNode) const
{
    for (auto it = nodePositions.constBegin(); it != nodePositions.constEnd(); ++it) {
        if (it->first == targetNode) {
            return it->second;
        }
    }
    return QPoint(-1, -1);
}

void TreeVisualizer::TreeDrawingWidget::drawTree(QPainter& painter, HuffmanNode* node,
                                                 const QPoint& parentPos, bool isLeftChild)
{
    if (!node) return;

    QPoint currentPos = getPositionForNode(node);

    if (parentPos.x() != -1) {
        // Draw line to parent
        painter.setPen(QPen(Qt::darkGray, 2));
        painter.drawLine(parentPos, currentPos);

        // Draw bit label (0 or 1)
        QPoint midPoint = (parentPos + currentPos) / 2;
        QString label = isLeftChild ? "0" : "1";

        painter.setBrush(Qt::white);
        painter.setPen(Qt::black);
        QRect labelRect(midPoint.x() - 15, midPoint.y() - 10, 30, 20);
        painter.drawRect(labelRect);
        painter.drawText(labelRect, Qt::AlignCenter, label);
    }

    // Draw children
    if (node->left) {
        drawTree(painter, node->left, currentPos, true);
    }
    if (node->right) {
        drawTree(painter, node->right, currentPos, false);
    }
}

void TreeVisualizer::TreeDrawingWidget::paintEvent(QPaintEvent *event)
{
    Q_UNUSED(event);

    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing, true);

    // Fill background
    painter.fillRect(rect(), Qt::white);

    if (!treeRoot || nodePositions.isEmpty()) {
        painter.setPen(Qt::black);
        painter.drawText(rect(), Qt::AlignCenter, "No tree to display");
        return;
    }

    // Draw the tree connections
    painter.setPen(QPen(Qt::darkGray, 2));
    drawTree(painter, treeRoot, QPoint(-1, -1), false);

    // Draw all nodes
    for (const auto& pair : nodePositions) {
        HuffmanNode* node = pair.first;
        QPoint pos = pair.second;
        QRect nodeRect(pos.x() - NODE_RADIUS, pos.y() - NODE_RADIUS,
                       NODE_RADIUS * 2, NODE_RADIUS * 2);

        // Choose color based on node type
        if (!node->left && !node->right) {
            // Leaf node - blue
            painter.setBrush(QColor(173, 216, 230));
            painter.setPen(QPen(QColor(0, 0, 139), 2));
        } else {
            // Internal node - green
            painter.setBrush(QColor(144, 238, 144));
            painter.setPen(QPen(QColor(0, 100, 0), 2));
        }

        // Draw node circle
        painter.drawEllipse(nodeRect);

        // Prepare node label
        QString label;
        if (!node->left && !node->right) {
            // Leaf: show character and frequency
            char ch = node->character;
            QString charStr;

            if (ch >= 32 && ch <= 126 && ch != '\\') {
                if (ch == ' ') {
                    charStr = "' '";
                } else {
                    charStr = QString("'%1'").arg(QChar(ch));
                }
            } else {
                charStr = QString("0x%1").arg((unsigned char)ch, 2, 16, QChar('0'));
            }

            label = QString("%1\n%2").arg(charStr).arg(node->frequency);
        } else {
            // Internal: show frequency only
            label = QString::number(node->frequency);
        }

        // Draw text
        painter.setPen(Qt::black);
        QFont font = painter.font();
        font.setPointSize(8);
        painter.setFont(font);
        painter.drawText(nodeRect, Qt::AlignCenter, label);
    }
}

// -------------------------------------------------------------
// TreeVisualizer (Main Dialog) implementation
// -------------------------------------------------------------

TreeVisualizer::TreeVisualizer(HuffmanNode* root, QWidget *parent)
    : QDialog(parent), treeRoot(root)
{
    setWindowTitle(tr("Huffman Tree Visualization"));
    setWindowFlags(windowFlags() | Qt::WindowMaximizeButtonHint);

    QVBoxLayout *mainLayout = new QVBoxLayout(this);

    // Create scroll area for large trees
    QScrollArea *scrollArea = new QScrollArea;
    scrollArea->setWidgetResizable(true);

    // Create the drawing widget
    drawingWidget = new TreeDrawingWidget(treeRoot);
    scrollArea->setWidget(drawingWidget);

    mainLayout->addWidget(scrollArea);

    // Add legend/info
    QLabel *infoLabel = new QLabel(
        tr("<b>Tree Legend:</b> "
           "<span style='color:darkgreen'>■</span> Internal Nodes (frequency) | "
           "<span style='color:darkblue'>■</span> Leaf Nodes (character + frequency) | "
           "Edges: 0 = left, 1 = right"));
    infoLabel->setAlignment(Qt::AlignCenter);
    infoLabel->setWordWrap(true);
    mainLayout->addWidget(infoLabel);

    // Add close button
    QPushButton *closeButton = new QPushButton(tr("Close"));
    connect(closeButton, &QPushButton::clicked, this, &QDialog::accept);
    mainLayout->addWidget(closeButton, 0, Qt::AlignCenter);

    // Set initial size
    resize(1000, 700);

    qDebug() << "TreeVisualizer created for root:" << treeRoot;
}

TreeVisualizer::~TreeVisualizer()
{
    // Note: Nodes are owned by Compressor/HuffmanTree
}
