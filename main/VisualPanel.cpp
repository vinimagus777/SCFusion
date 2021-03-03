#include "VisualPanel.h"

BEGIN_EVENT_TABLE(VisualPanel, wxScrolledWindow)
END_EVENT_TABLE()

VisualPanel::VisualPanel(wxFrame* parent, wxWindowID id) :
    wxScrolledWindow(parent, id, wxDefaultPosition, wxDefaultSize, wxBORDER_THEME)
{
    SetBackgroundColour(wxColour(255, 255, 255));
    SetScrollbars(10, 10, 200, 200, 0, 0);
}

const int PIXELS_PER_SECOND = 5;
const int LABEL_OFFSET_LEFT = 3;
const int LABEL_OFFSET_TOP = 2;
const int OFFSET_TOP = 30;
const int ROW_HEIGHT = 30;
const int ITEM_HEIGHT = 20;
const int STATUS_HEIGHT = 5;
const int DOUBLE_QUEUE_ITEM_HEIGHT = ROW_HEIGHT / 2;
const int doubleQueueMarginCorrection = (ITEM_HEIGHT - ROW_HEIGHT) / 2;

bool compareStartTime(VisualItem a, VisualItem b)
{
    return a.startTime < b.startTime;
}

void VisualPanel::SetVisualItems(vector<vector<VisualItem>> visualItems)
{
    int maxEndTime = 0;

    // split stray items (not occupying a building) on multiple rows to avoid overlapping
    m_stray_visual_items.clear();
    if (visualItems.size() > 0)
    {
        vector<VisualItem> strayRow = visualItems[0];
        sort(strayRow.begin(), strayRow.end(), compareStartTime);
        for (size_t i = 0; i < strayRow.size(); i++)
        {
            // track max endTime
            if (strayRow[i].endTime > maxEndTime) maxEndTime = strayRow[i].endTime;

            bool fitted = false;
            size_t n = 0;
            while (!fitted && n < m_stray_visual_items.size())
            {
                if (m_stray_visual_items[n].back().endTime > strayRow[i].startTime || m_stray_visual_items[n].back().startTime + 10 > strayRow[i].startTime)
                {
                    n++;
                }
                else
                {
                    m_stray_visual_items[n].push_back(strayRow[i]);
                    fitted = true;
                }
            }
            
            if (!fitted)
            {
                // add new row for stray item
                vector<VisualItem> row;
                row.push_back(strayRow[i]);
                m_stray_visual_items.push_back(row);
            }
        }

        visualItems[0].clear();
    }

    m_visual_items = visualItems;
    m_height = (m_visual_items.size() + m_stray_visual_items.size()) * 30 + 50;
    
    for (size_t i = 0; i < m_visual_items.size(); i++)
    {
        if (m_visual_items[i].size() && m_visual_items[i].back().endTime > maxEndTime) maxEndTime = m_visual_items[i].back().endTime;
    }
    m_width = maxEndTime * 5 + 50;

    wxPoint scrolled = GetViewStart();
    SetScrollbars(10, 10, (int)m_width/10, (int)m_height/10, scrolled.x, scrolled.y);
}

wxColor VisualPanel::GetBrushColorByType(VisualItem::VisualItemType itemType)
{
    const wxColor DEFAULT = wxColor(204, 204, 204);
    const wxColor BASE = wxColor(153, 204, 255);
    const wxColor GAS = wxColor(153, 204, 153);
    const wxColor SUPPLY = wxColor(255, 204, 153);
    const wxColor STATUS = wxColor(153, 255, 255);
    const wxColor MILITARY = wxColor(255, 153, 153);
    const wxColor MILITARY_UNIT = wxColor(255, 204, 204);
    const wxColor WORKER = wxColor(204, 229, 255);
    const wxColor RESEARCH = wxColor(229, 204, 255);
    
    switch (itemType)
    {
    case VisualItem::tBase:
        return BASE;
    case VisualItem::tGas:
        return GAS;
    case VisualItem::tSupply:
        return SUPPLY;
    case VisualItem::tStatus:
        return STATUS;
    case VisualItem::tMilitary:
        return MILITARY;
    case VisualItem::tMilitaryUnit:
        return MILITARY_UNIT;
    case VisualItem::tWorker:
        return WORKER;
    case VisualItem::tResearch:
        return RESEARCH;
    default:
        return DEFAULT;
    }

}

void VisualPanel::OnDraw(wxDC& dc)
{
    // Draw grid
    dc.SetPen(wxPen(wxColor(230, 230, 230), 1));
    dc.SetTextForeground(wxColor(153, 153, 153));
    for (int i = 1; i <= m_width / 15 / PIXELS_PER_SECOND; i++)
    {
        dc.DrawLine(wxPoint(i * 15 * PIXELS_PER_SECOND, 20), wxPoint(i * 15 * PIXELS_PER_SECOND, m_height - 20));
        if (i % 4 == 0) dc.DrawLine(wxPoint(i * 15 * PIXELS_PER_SECOND - 1, 20), wxPoint(i * 15 * PIXELS_PER_SECOND - 1, m_height - 20));
        dc.DrawText(wxString::Format(L"%02d:%02d", i / 4, i % 4 * 15), i * 15 * PIXELS_PER_SECOND - 13, 3);
        dc.DrawText(wxString::Format(L"%02d:%02d", i / 4, i % 4 * 15), i * 15 * PIXELS_PER_SECOND - 13, m_height - 17);
    }

    // Draw items
    dc.SetBrush(wxColor(127, 191, 255));
    dc.SetPen(*wxTRANSPARENT_PEN);
    dc.SetTextForeground(wxColor(0, 0, 0));

    for (size_t i = 0; i < m_stray_visual_items.size(); i++)
    {
        for (auto item : m_stray_visual_items[i])
        {
            dc.SetBrush(GetBrushColorByType(item.itemType));
            dc.DrawRectangle(
                item.startTime * PIXELS_PER_SECOND,
                i * ROW_HEIGHT + OFFSET_TOP,
                (item.endTime - item.startTime) * PIXELS_PER_SECOND - 1,
                ITEM_HEIGHT - 1
            );
            dc.DrawText(item.name, item.startTime * PIXELS_PER_SECOND + LABEL_OFFSET_LEFT, i * ROW_HEIGHT + OFFSET_TOP + LABEL_OFFSET_TOP);
        }
    }

    for (size_t i = 0; i < m_visual_items.size(); i++)
    {
        size_t offset = i + m_stray_visual_items.size() - 1;
        bool doubleQueueUpper = false;
        for (auto item : m_visual_items[i])
        {
            dc.SetBrush(GetBrushColorByType(item.itemType));
            if (item.itemType == VisualItem::tStatus)
            {
                dc.DrawRectangle(
                    item.startTime * PIXELS_PER_SECOND,
                    offset * ROW_HEIGHT + OFFSET_TOP + ITEM_HEIGHT,
                    (item.endTime - item.startTime) * PIXELS_PER_SECOND - 1,
                    STATUS_HEIGHT - 1
                );
            }
            else
            {
                if (item.isDoubleQueue)
                {
                    doubleQueueUpper = !doubleQueueUpper;
                    dc.DrawRectangle(
                        item.startTime * PIXELS_PER_SECOND,
                        offset * ROW_HEIGHT + OFFSET_TOP + (doubleQueueUpper ? 0 : DOUBLE_QUEUE_ITEM_HEIGHT) + doubleQueueMarginCorrection,
                        (item.endTime - item.startTime) * PIXELS_PER_SECOND - 1,
                        DOUBLE_QUEUE_ITEM_HEIGHT - 1
                    );
                    dc.DrawText(
                        item.name,
                        item.startTime * PIXELS_PER_SECOND + LABEL_OFFSET_LEFT,
                        offset * ROW_HEIGHT + OFFSET_TOP + (doubleQueueUpper ? 0 : DOUBLE_QUEUE_ITEM_HEIGHT) + doubleQueueMarginCorrection - 1);
                }
                else
                {
                    dc.DrawRectangle(
                        item.startTime * PIXELS_PER_SECOND,
                        offset * ROW_HEIGHT + OFFSET_TOP,
                        (item.endTime - item.startTime) * PIXELS_PER_SECOND - 1,
                        ITEM_HEIGHT - 1
                    );
                    dc.DrawText(item.name, item.startTime * PIXELS_PER_SECOND + LABEL_OFFSET_LEFT, offset * ROW_HEIGHT + OFFSET_TOP + LABEL_OFFSET_TOP);
                }
            }
        }
    }
}