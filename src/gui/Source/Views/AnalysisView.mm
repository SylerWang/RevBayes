#import "AnalysisDocument.h"
#import "AnalysisView.h"
#import "ConnectionPair.h"
#import "InOutlet.h"
#import "ScalingScrollView.h"
#import "Tool.h"
#import "ToolAlign.h"
#import "ToolNumericalMcmcOutput.h"
#import "ToolMatrixFilter.h"
#import "ToolMcmc.h"
#import "ToolMcmcDiagnostic.h"
#import "ToolModel.h"
#import "ToolReadData.h"
#import "ToolSimulate.h"
#import "ToolTreeSet.h"
#import "ToolView.h"
#import "WindowControllerPreferences.h"



@implementation AnalysisView

- (void)dealloc {

	[super dealloc];
}

- (void)drawRect:(NSRect)dirtyRect {
    	
	// set the background of the analysis window
	NSRect bounds = [self bounds];
	[bkgrndColor set];
	[NSBezierPath fillRect:bounds];
    
    // set the grid
    if ( showGrid == YES )
        {
        float x = 0.0;
        while (x < bounds.size.width)
            {
            x += gridSize * scaleFactor;
            NSPoint p1 = NSMakePoint(x, 0.0);
            NSPoint p2 = NSMakePoint(x, bounds.size.height);
            NSBezierPath* connection = [NSBezierPath bezierPath];
            [connection moveToPoint:p1];
            [connection lineToPoint:p2];
			[gridColor set];
			[connection setLineWidth:0.2];
            [connection stroke];
            }
            
        float y = bounds.size.height;
        while (y > 0.0)
            {
            y -= gridSize * scaleFactor;
            NSPoint p1 = NSMakePoint(0.0, y);
            NSPoint p2 = NSMakePoint(bounds.size.width, y);
            NSBezierPath* connection = [NSBezierPath bezierPath];
            [connection moveToPoint:p1];
            [connection lineToPoint:p2];
			[gridColor set];
			[connection setLineWidth:0.2];
            [connection stroke];
            }
        }

	// get pointers of images to be drawn over items
    NSImage* magnifyingImageOff = [NSImage imageNamed:@"Unselected_Magnifier.icns"];
    [magnifyingImageOff setSize:NSMakeSize(ITEM_IMAGE_SIZE*scaleFactor*0.2, ITEM_IMAGE_SIZE*scaleFactor*0.2)];

    // instantiate a shadow object
    NSShadow* shadow = [[NSShadow alloc] init];
    [shadow setShadowOffset:NSMakeSize(3.0, -2.0)];
    [shadow setShadowBlurRadius:10.0];
    [NSGraphicsContext saveGraphicsState];
    [shadow set];
	
	// draw the tool images
	NSEnumerator* itemEnumerator = [itemsPtr objectEnumerator];
	id element;
	while ( (element = [itemEnumerator nextObject]) )
		{
		// get information on the location and size of the tool's image
		NSImage* itemImage = [element itemImageWithIndex:scaleIdx];
		NSRect imageRect;
		imageRect.origin = NSZeroPoint;
		imageRect.size = [itemImage size];
		NSRect drawingRect = imageRect;
		drawingRect.origin = [element itemLocation];
        drawingRect.size = NSMakeSize(ITEM_IMAGE_SIZE*scaleFactor, ITEM_IMAGE_SIZE*scaleFactor);
		[self transformToBottomLeftCoordinates:(&drawingRect.origin)];            
        
		// draw a focus ring behind the tool, if it is currently selected
        if ([element isSelected] == YES)
            {
            NSRect focusRect;
            focusRect = drawingRect;
            focusRect.origin.x -= 1.0;
            focusRect.origin.y -= 1.0;
            focusRect.size.height += 2.0;
            focusRect.size.width += 2.0;
            [[NSColor keyboardFocusIndicatorColor] set];

            NSBezierPath* selRing = [NSBezierPath bezierPath];
            float rad = focusRect.size.width * 0.1;
            [selRing appendBezierPathWithRoundedRect:focusRect xRadius:rad yRadius:rad];
            [selRing fill];
            }

        // flag the object, with a red square behind the image that gets bigger or smaller depending upon the flag count
		if ([element flagCount] > 0)
			{
			float factor = 1.0;
			if ([element flagCount] % 2 == 0)
				factor = 4.0;
            NSRect flagRect;
            flagRect = drawingRect;
            flagRect.origin.x -= 1.0*factor;
            flagRect.origin.y -= 1.0*factor;
            flagRect.size.height += 2.0*factor;
            flagRect.size.width += 2.0*factor;

            [[NSColor redColor] set];

            NSBezierPath* selRing = [NSBezierPath bezierPath];
            float rad = flagRect.size.width * 0.1;
            [selRing appendBezierPathWithRoundedRect:flagRect xRadius:rad yRadius:rad];
            [selRing fill];
			}

		// draw the tool image
		[itemImage drawInRect:drawingRect fromRect:imageRect operation:NSCompositeSourceOver fraction:1.0];
        		
		// draw the magnifying glass in the lower-left corner of the tool
        if ([element hasInspectorInfo] == YES)
            {
			NSRect infoImageRect;
			infoImageRect.origin = NSZeroPoint;
			infoImageRect.size = [magnifyingImageOff size];
			NSRect mRect = informationRect[scaleIdx];
			mRect.origin = NSMakePoint(drawingRect.origin.x + mOffset[scaleIdx].x, drawingRect.origin.y + mOffset[scaleIdx].y);
            if ( [element isCursorOver] == YES || (draggedItems == YES && element == selection.selectedItem) )
				{
				[[[NSColor blackColor] colorWithAlphaComponent:0.25] set];
				[[NSBezierPath bezierPathWithOvalInRect:mRect] fill];
                [magnifyingImageOff drawInRect:mRect fromRect:infoImageRect operation:NSCompositeSourceOver fraction:1.0];
				}
            else 
				{
                [magnifyingImageOff drawInRect:mRect fromRect:infoImageRect operation:NSCompositeSourceOver fraction:1.0];
				}
            }

		// draw the information button in the lower-right corner of the tool
		NSRect iRect = informationRect[scaleIdx];
		iRect.origin = NSMakePoint(drawingRect.origin.x + rOffset[scaleIdx].x, drawingRect.origin.y + rOffset[scaleIdx].y);
		NSPoint p    = NSMakePoint(drawingRect.origin.x + iOffset[scaleIdx].x, drawingRect.origin.y + iOffset[scaleIdx].y);
		if ( [element isCursorOver] == YES || (draggedItems == YES && element == selection.selectedItem) )
			{
			[[[NSColor blackColor] colorWithAlphaComponent:0.25] set];
			[[NSBezierPath bezierPathWithOvalInRect:iRect] fill];
			[selectedAttributedString[scaleIdx] drawAtPoint:p];
			}
		else 
			{
			[unselectedAttributedString[scaleIdx] drawAtPoint:p];
			}

        }

    // draw the connections
	itemEnumerator = [itemsPtr objectEnumerator];
	while ( (element = [itemEnumerator nextObject]) )
		{
		for (int i=0; i<[element numOutlets]; i++)
			{
			InOutlet* ol = [element outletIndexed:i];
			if ([ol partner] != nil)
				{
				Tool* t1 = [ol toolOwner];
				Tool* t2 = [[ol partner] toolOwner];
				
				NSRect drawingRect;
				drawingRect.origin = [t1 itemLocation];
				drawingRect.size = [[t1 itemImageWithIndex:scaleIdx] size];
				[self transformToBottomLeftCoordinates:(&drawingRect.origin)];
				NSPoint p1 = [ol getInOutletPointFromPoint:drawingRect.origin];

				drawingRect.origin = [t2 itemLocation];
				[self transformToBottomLeftCoordinates:(&drawingRect.origin)];
				NSPoint p2 = [[ol partner] getInOutletPointFromPoint:drawingRect.origin];
				
				NSPoint i1 = p1;
				NSPoint i2 = p2;
				float x = (p1.x - p2.x) * 0.5;
				if (x < 0.0)
					x = p1.x - x;
				else 
					x = p2.x + x;
				i1.x = x;
				i2.x = x;

				NSBezierPath* connection = [NSBezierPath bezierPath];
				[connection setLineJoinStyle:NSRoundLineJoinStyle];
				[connection moveToPoint:p1];
				[connection lineToPoint:i1];
				[connection lineToPoint:i2];
				[connection lineToPoint:p2];
				[[ol toolColor] set];
				if ([ol isSelected] == YES)
					[connection setLineWidth:(6.0 * scaleFactor)];
				else
					[connection setLineWidth:(4.0 * scaleFactor)];
				[connection stroke];
				}
			}
        }

    // draw the inlets and outlets
    itemEnumerator = [itemsPtr objectEnumerator];
	while ( (element = [itemEnumerator nextObject]) )
		{
		NSImage* itemImage = [element itemImageWithIndex:scaleIdx];
		NSRect imageRect;
		imageRect.origin = NSZeroPoint;
		imageRect.size = [itemImage size];
		NSRect drawingRect = imageRect;
		drawingRect.origin = [element itemLocation];
		drawingRect.size = [itemImage size];
		[self transformToBottomLeftCoordinates:(&drawingRect.origin)];
		
		// draw the inlets
		for (int i=0; i<[element numInlets]; i++)
			{
			InOutlet* il = [element inletIndexed:i];
			NSRect r = [il getInOutletRectFromPoint:drawingRect.origin];
			NSPoint p1 = NSMakePoint(r.origin.x, r.origin.y);
			NSPoint p2 = NSMakePoint(r.origin.x, r.origin.y + r.size.height);
			NSPoint p3 = NSMakePoint(r.origin.x + r.size.width, r.origin.y + 0.5*r.size.height);
			NSBezierPath *triangle = [NSBezierPath bezierPath];
			[triangle moveToPoint:p1];
			[triangle lineToPoint:p2];
			[triangle lineToPoint:p3];
			[triangle lineToPoint:p1];
			[[element colorOfInletIndexed:i] set];
			[triangle fill];
            [[NSColor blackColor] set];
            [triangle stroke];
			}
		
		// draw the outlets
		for (int i=0; i<[element numOutlets]; i++)
			{
			InOutlet* ol = [element outletIndexed:i];
			NSRect r = [ol getInOutletRectFromPoint:drawingRect.origin];
			NSPoint p1 = NSMakePoint(r.origin.x, r.origin.y);
			NSPoint p2 = NSMakePoint(r.origin.x, r.origin.y + r.size.height);
			NSPoint p3 = NSMakePoint(r.origin.x + r.size.width, r.origin.y + 0.5*r.size.height);
			NSBezierPath *triangle = [NSBezierPath bezierPath];
			[triangle moveToPoint:p1];
			[triangle lineToPoint:p2];
			[triangle lineToPoint:p3];
			[triangle lineToPoint:p1];
			[[element colorOfOutletIndexed:i] set];
			[triangle fill];
            [[NSColor blackColor] set];
            [triangle stroke];
			}
		}
        
    [NSGraphicsContext restoreGraphicsState];
        
    // draw the tool tip
    itemEnumerator = [itemsPtr objectEnumerator];
	while ( (element = [itemEnumerator nextObject]) )
        {
        if ([element showTip] == YES)
            {
			NSPoint p = [element itemCenter];
			[self transformToBottomLeftCoordinates:(&p)];
            
            NSMutableAttributedString* attrString = [element sendTip];
			NSRect tipRect = [attrString boundingRectWithSize:NSMakeSize(1e10, 1e10) options:(NSStringDrawingUsesLineFragmentOrigin)];
            tipRect.origin = p;
            
            NSRect visible = [self visibleRect];
            int i = 0;
            while ( NSContainsRect(visible, tipRect) == NO && i <= 3 ) 
                {
                if (i == 0)
                    tipRect.origin.x -= tipRect.size.width;
                else if (i == 1)
                    tipRect.origin.y -= tipRect.size.height;
                else if (i == 3)
                    tipRect.origin.x += tipRect.size.width;
                p = tipRect.origin;
                i++;
                } 
            
            [[[NSColor darkGrayColor] colorWithAlphaComponent:0.75] set];
            [NSBezierPath fillRect:tipRect];
			[attrString drawAtPoint:p];
			[attrString release];
            }
        }
        
	// draw an active drag between inlet/outlet
	if ( selection.selectedItem != nil && (selection.selectionType == INLET_SELECTION || selection.selectionType == OUTLET_SELECTION) )
		{
		InOutlet* ilol = selection.selectedItem;
			
		Tool* t = [ilol toolOwner];
		NSRect drawingRect;
		drawingRect.origin = [t itemLocation];
		drawingRect.size = [[t itemImageWithIndex:scaleIdx] size];
		[self transformToBottomLeftCoordinates:(&drawingRect.origin)];

		NSPoint p1 = [ilol getInOutletPointFromPoint:drawingRect.origin];
		NSPoint p2 = cursorLocation;

		NSPoint i1 = p1;
		NSPoint i2 = p2;
		float x = (p1.x - p2.x) * 0.5;
		if (x < 0.0)
			x = p1.x - x;
		else 
			x = p2.x + x;
		i1.x = x;
		i2.x = x;
		
		NSBezierPath* connection = [NSBezierPath bezierPath];
		[connection moveToPoint:p1];
		[connection lineToPoint:i1];
		[connection lineToPoint:i2];
		[connection lineToPoint:p2];
		[[ilol toolColor] set];
		[connection setLineWidth:(4.0 * scaleFactor)];
		[connection stroke];
		}
		
	// draw a sweep for multiple selection
	if (sweepAction == YES)
		{
		NSRect sweepRect;

		NSPoint p1 = sweepStartLocation;
		NSPoint p2 = sweepCurrentLocation;
		sweepRect.origin = p1;
		if (p2.x < p1.x)
			sweepRect.origin.x = p2.x;
		if (p2.y < p1.y)
			sweepRect.origin.y = p2.y;
		sweepRect.size.width = p1.x - p2.x;
		sweepRect.size.height = p1.y - p2.y;
		if (sweepRect.size.width < 0.0)
			sweepRect.size.width = -sweepRect.size.width;
		if (sweepRect.size.height < 0.0)
			sweepRect.size.height = -sweepRect.size.height;

		NSColor* sweepColor = [NSColor whiteColor];
		sweepColor = [sweepColor colorWithAlphaComponent:0.3];
		[sweepColor set];
		[NSBezierPath fillRect:sweepRect];
		}
}

- (id)initWithFrame:(NSRect)frame {

    if ( (self = [super initWithFrame:frame]) ) 
		{        
        // add self as observer for scale factor change
		NSNotificationCenter *defaultCenter = [NSNotificationCenter defaultCenter];		 
		[defaultCenter addObserver:self
						  selector:@selector(scaleFactorChanged:)
							  name:@"scaleFactorChangeNotification"
						    object:nil];

        // get the background color and grid color from the defaults
        NSUserDefaults* defaults = [NSUserDefaults standardUserDefaults];
        NSData* colorAsData = [defaults objectForKey:RB_AnalysisBgrndColorKey];
        bkgrndColor = [NSKeyedUnarchiver unarchiveObjectWithData:colorAsData];
        [bkgrndColor retain];
        colorAsData = [defaults objectForKey:RB_AnalysisGridColorKey];
        gridColor = [NSKeyedUnarchiver unarchiveObjectWithData:colorAsData];
        [gridColor retain];
		}
    return self;
}

- (void)mouseDown:(NSEvent*)event {
	
	// inactivate the timer, if it is still going
	if ([analysisDocumentPtr isRbTimerActive] == YES)
		[analysisDocumentPtr invalidateTimer];
	[self stopItemTipTimer];
    	
	// see which item might have been selected
	ItemSelector mySelection = [self selectItem:[event locationInWindow]];
	selection = mySelection;
	
    shiftClicked = NO;
    draggedItems = NO;
	sweepAction  = NO;
	if ([event modifierFlags] & NSControlKeyMask)
		{
        // control click
		if ( mySelection.selectedItem != nil && (mySelection.selectionType == ITEM_SELECTION || mySelection.selectionType == INFO_SELECTION || mySelection.selectionType == INSP_SELECTION) )
            [mySelection.selectedItem showControlPanel];
		}
	else if ([event modifierFlags] & NSAlternateKeyMask)
		{
        // option click (do nothing as option click is currently undefined for tools)
		}
	else if ([event modifierFlags] & NSShiftKeyMask)
		{
        // shift click
		shiftClicked = YES;
		if (mySelection.selectedItem != nil)
			{
			if (mySelection.selectionType == ITEM_SELECTION || mySelection.selectionType == LINK_SELECTION || mySelection.selectionType == INFO_SELECTION || mySelection.selectionType == INSP_SELECTION)
				[mySelection.selectedItem flipSelected];
			}
		else 
			{
			sweepStartLocation = [self convertPoint:[event locationInWindow] fromView:nil];
			[self removeAllFocusRings];
			}
		}
	else
		{	
        // click, with no modifier keys
		if (mySelection.selectedItem != nil)
			{
			if (mySelection.selectionType == INLET_SELECTION || mySelection.selectionType == OUTLET_SELECTION)
				[[NSCursor crosshairCursor] push];
			else if (mySelection.selectionType == ITEM_SELECTION)
				[mySelection.selectedItem setIsSelected:YES];
			else if (mySelection.selectionType == INFO_SELECTION)
                [mySelection.selectedItem showControlPanel];
			else if (mySelection.selectionType == INSP_SELECTION)
                [mySelection.selectedItem showInspectorPanel];

			NSPoint curP = [[self window] mouseLocationOutsideOfEventStream];
			[self selectCursorOverForItemsAtPoint:[self convertPoint:curP fromView:nil]];
			}
		else 
			{
			sweepStartLocation = [self convertPoint:[event locationInWindow] fromView:nil];
			[self removeAllFocusRings];
			}
		}
}

- (void)mouseDragged:(NSEvent*)event {
	
    // get the position of the cursor
	NSPoint p = [event locationInWindow];
	if (selection.selectedItem != nil)
		{
		if (selection.selectionType == ITEM_SELECTION)
			{
            // recalculate the item's coordinates: we are dragging the item
            if (draggedItems == NO)
                {
                // update information that we want to do only once during a drag operation
				// change the cursor
                [[NSCursor openHandCursor] push];
				
				// make certain selected items drag properly together
				if (shiftClicked == NO)
					[self removeAllFocusRings];
				[selection.selectedItem setIsSelected:YES];
                }
			draggedItems = YES;
                            
            // convert the point to the view's coordinates
            NSPoint newP = [self convertPoint:p fromView:nil];
            [self transformToTopLeftCoordinates:(&newP)];
            
            // account for the initial point where the item was selected
            newP.x -= cursorOffset.width;
            newP.y += cursorOffset.height;

			// find the nearest grid intersection to this point, if we are snapping the item to the grid
            if (snapToGrid == YES)
                newP = [self findNearestGridIntersectionToPoint:newP];
            
            // get the item's old location
            NSPoint oldP = [selection.selectedItem itemLocation];
			
			// get the change in the x- and y-values
			float xChange = newP.x - oldP.x;
			float yChange = newP.y - oldP.y;
            
            // set new location for all of the selected items
            NSEnumerator* enumerator = [itemsPtr objectEnumerator];
            id element;
			BOOL isCollided = NO;
            while ( (element = [enumerator nextObject]) )
				{
				if ([element isSelected] == YES)
					{
                    NSPoint np = [element itemLocation];
                    np.x += xChange;
                    np.y += yChange;
                    [element setItemLocation:np];
					if ([self hasItemCollided:element] == YES)
						isCollided = YES;
					}
				}
            
            // reset the bottom right corner
            [self setCorners];
			
			// check for collisions
			if ( topLeftCorner.x < 0.0 || topLeftCorner.y < 0.0 || isCollided == YES )
				{
				enumerator = [itemsPtr objectEnumerator];
				while ( (element = [enumerator nextObject]) )
					{
					if ([element isSelected] == YES)
						{
						NSPoint np = [element itemLocation];
						np.x -= xChange;
						np.y -= yChange;
						[element setItemLocation:np];
						}
					}
				}
            
            // update the scroll bars
            [self updateScrollBars];
			}
		else 
			{
			NSPoint newP = [self convertPoint:p fromView:nil];
			cursorLocation = newP;
			}
		}
	else 
		{
        // we are performing a multiple-selection sweep
        if (sweepAction == NO)
            {
            // get the list of items originally selected
            [selectedItems removeAllObjects];
            NSEnumerator* enumerator = [itemsPtr objectEnumerator];
            id element;
            while ( (element = [enumerator nextObject]) )
                {
                if ([element isSelected] == YES)
                    [selectedItems addObject:element];
                for (int i=0; i<[element numOutlets]; i++)
                    {
                    InOutlet* ol = [element outletIndexed:i];
                    if ([ol partner] != nil && [ol isSelected] == YES)
                        [selectedItems addObject:ol];
                    }
                }
            }
		sweepAction = YES;
		sweepCurrentLocation = [self convertPoint:p fromView:nil];
        [self selectItemsInSweepArea];
		}
	[self autoscroll:event];
	[self setNeedsDisplay: YES];	
}

- (void)mouseUp:(NSEvent*)event {

	if (selection.selectedItem != nil)
		{
		if (selection.selectionType == ITEM_SELECTION)
			{
			// we had a mouse up on a selected item
            if (draggedItems == NO && shiftClicked == NO)
                {
                [self removeAllFocusRings];
                [selection.selectedItem setIsSelected:YES];
                }
            if (draggedItems == YES)
                {
                // we finished moving a item
                
                // update the cursor
                [[NSCursor openHandCursor] pop];

                // update the tracking areas
                NSEnumerator* enumerator = [itemsPtr objectEnumerator];
                id element;
                while ( (element = [enumerator nextObject]) )
                    {
					[self addTrackingForItem:element];
                    }
				[self mouseEntered:event];
                }
			}
		else if (selection.selectionType == LINK_SELECTION)
			{
            if (draggedItems == NO && shiftClicked == NO)
                {
                [self removeAllFocusRings];
                [selection.selectedItem setIsSelected:YES];
                }
			}
		else if (selection.selectionType == INFO_SELECTION)
            {
            
            }
		else if (selection.selectionType == INSP_SELECTION)
            {
            
            }
		else 
			{
			// we attempt to finish establishing a connection b/w inlet/outlets
			InOutlet* start = selection.selectedItem;
				
			NSPoint p = [event locationInWindow];
			ItemSelector mouseUpSelection = [self selectItem:p];

			if (mouseUpSelection.selectedItem != nil && mouseUpSelection.selectedItem != [start toolOwner] && mouseUpSelection.selectedItem != start)
				{
				// get the other end of the connection
				InOutlet* end = nil;
				if (mouseUpSelection.selectionType == INLET_SELECTION || mouseUpSelection.selectionType == OUTLET_SELECTION)
					end = mouseUpSelection.selectedItem;
				else if (mouseUpSelection.selectionType == ITEM_SELECTION)
					end = [mouseUpSelection.selectedItem findInOutletWithColor:[start toolColor]];
					
				// are both start and end inlets or are both outlets?
				BOOL sameInOutType = NO;
				if ( !([start amInlet] != [end amInlet]) )
					sameInOutType = YES;
					
				// are they the same tool type?
				BOOL sameClassType = NO;
				if (end != nil)
					{
					Tool* t1 = [start toolOwner];
					Tool* t2 = [end toolOwner];
					NSString* className1 = NSStringFromClass([t1 class]); 
					NSString* className2 = NSStringFromClass([t2 class]); 
					if ( [className1 isEqualToString:className2] == YES )
						sameClassType = YES;
					}
				
				// are they the same color?
				BOOL sameColor = YES;
				if ( [start toolColor] != [end toolColor] )
					sameColor = NO;
					
				// make the connection
				if ( end != nil && start != end && [start amInlet] != [end amInlet] && [start toolColor] == [end toolColor] && [start toolOwner] != [end toolOwner] && sameClassType == NO )
					{
                    if ([start partner] != nil)
                        {
                        [[start partner] setPartner:nil];
                        [start setPartner:nil];
                        }
                    if ([end partner] != nil)
                        {
                        [[end partner] setPartner:nil];
                        [end setPartner:nil];
                        }
					[start setPartner:end];
					[end setPartner:start];
					if ( [start amInlet] == NO )
						[[end toolOwner] updateForConnectionChange];
					else if ( [end amInlet] == NO )
						[[start toolOwner] updateForConnectionChange];
					else 
						NSLog(@"Error: Problem informing neighbors of new connection");
					}
					
				// inform the user of certain errors
				if (sameInOutType == YES)
					{
					NSAlert* alert = [NSAlert alertWithMessageText:@"Tool Connection Warning" 
													 defaultButton:@"OK" 
												   alternateButton:nil 
													   otherButton:nil 
										 informativeTextWithFormat:@"You must connect outlets to inlets."];
					[alert beginSheetModalForWindow:[self window] modalDelegate:self didEndSelector:nil contextInfo:NULL];
					}
				else if (sameColor == NO)
					{
					NSAlert* alert = [NSAlert alertWithMessageText:@"Tool Connection Warning" 
													 defaultButton:@"OK" 
												   alternateButton:nil 
													   otherButton:nil 
										 informativeTextWithFormat:@"You must connect outlets with inlets of the same color."];
					[alert beginSheetModalForWindow:[self window] modalDelegate:self didEndSelector:nil contextInfo:NULL];
					}
				else if (sameClassType == YES)
					{
					NSAlert* alert = [NSAlert alertWithMessageText:@"Tool Connection Warning" 
													 defaultButton:@"OK" 
												   alternateButton:nil 
													   otherButton:nil 
										 informativeTextWithFormat:@"You cannot connect tools of the same type."];
					[alert beginSheetModalForWindow:[self window] modalDelegate:self didEndSelector:nil contextInfo:NULL];
					}
				}
			[[NSCursor crosshairCursor] pop];
			}
        [[[NSDocumentController sharedDocumentController] currentDocument] updateChangeCount:NSChangeDone];
		}
    else 
        {
        if (sweepAction == YES)
            {
            // we clean up a sweep action
            [selectedItems removeAllObjects];
            }
        }
		
    // put the selection back to a base state
	selection.selectedItem = nil;
    draggedItems = NO;
	shiftClicked = NO;
	sweepAction  = NO;
    
    // update the display
	[self setNeedsDisplay: YES];
}

- (BOOL)performDragOperation:(id <NSDraggingInfo>)sender {

	if ( [sender draggingSource] != self )
		{
		// which tool was selected?
		int toolIdx = [[sender draggingSource] whichToolSelected];
		
		// get the location of the drop and then convert it to the view's coordinate system
		NSPoint p = [sender draggedImageLocation];
		NSPoint newP = [self convertPoint:p fromView:nil];
		p = newP;
		[self transformToTopLeftCoordinates:(&p)];

		// find the nearest grid intersection to this point, if we are snapping the tool to the grid
		if (snapToGrid == YES)
			p = [self findNearestGridIntersectionToPoint:p];
		
		// check that the drop location is OK
		NSRect r;
		r.origin = p;
        r.size   = NSMakeSize(ITEM_IMAGE_SIZE*scaleFactor, ITEM_IMAGE_SIZE*scaleFactor);
		if ( [self hasRectCollided:r] == YES)
			return NO;
		
		// allocate the tool, set its location, and add it to the list of tools
        Tool* newTool;
        if (toolIdx == 0)
            newTool = [[ToolReadData alloc] initWithScaleFactor:scaleFactor];
        else if (toolIdx == 1)
            newTool = [[ToolAlign alloc] initWithScaleFactor:scaleFactor];
        else if (toolIdx == 2)
            newTool = [[ToolModel alloc] initWithScaleFactor:scaleFactor];
        else if (toolIdx == 3)
            newTool = [[ToolMcmc alloc] initWithScaleFactor:scaleFactor];
        else if (toolIdx == 4)
            newTool = [[ToolSimulate alloc] initWithScaleFactor:scaleFactor];
        else if (toolIdx == 5)
            newTool = [[ToolTreeSet alloc] initWithScaleFactor:scaleFactor];
        else if (toolIdx == 6)
            newTool = [[ToolMatrixFilter alloc] initWithScaleFactor:scaleFactor];
        else if (toolIdx == 7)
            newTool = [[ToolNumericalMcmcOutput alloc] initWithScaleFactor:scaleFactor];
        else if (toolIdx == 8)
            newTool = [[ToolMcmcDiagnostic alloc] initWithScaleFactor:scaleFactor];

        // set the location for the tool
		[newTool setItemLocation:p];
		
		// add the tool to the tracking area
		[self addTrackingForItem:newTool];
		[newTool setIsCursorOver:YES];
        itemArea.selectedItem = newTool;

        // add the tool to the array of tools
		[itemsPtr addObject:newTool];
        [newTool release];
		
		// reset the bottom right corner
		[self setCorners];
		
        // set an update flag to the document
        [[[NSDocumentController sharedDocumentController] currentDocument] updateChangeCount:NSChangeDone];

		// update the display
		[self setNeedsDisplay:YES];
		}
	return YES;
}

- (BOOL)readFromPasteboard:(NSPasteboard*)pb {

	if ( [itemsPtr count] == 0 )
		pasteNum = 1;
		
    NSData* d = [pb dataForType:ToolDataPboardType];
    NSMutableArray* pastedTools = [NSKeyedUnarchiver unarchiveObjectWithData:d];

	NSEnumerator* toolEnumerator = [pastedTools objectEnumerator];
	id element;
	while ( (element = [toolEnumerator nextObject]) )
        {
        // offset the tools location a bit
        NSPoint p = [element itemLocation];
		p.x += (5.0) * pasteNum;
		p.y += ([element itemSize].width + 5.0) * pasteNum;
		if (snapToGrid == YES)
			p = [self findNearestGridIntersectionToPoint:p];
        [element setItemLocation:p];
        
		// add the tool to the tracking area
		[self addTrackingForItem:element];
		[element setIsCursorOver:NO];

        // add the tool to the array of tools
		[itemsPtr addObject:element];
        //[element release];
		
		// reset the bottom right corner
		[self setCorners];
        [self updateScrollBars];
        }

	// update the tool's state
	toolEnumerator = [pastedTools objectEnumerator];
	while ( (element = [toolEnumerator nextObject]) )
        {
		[element updateForConnectionChange];
		}
        
	// update the display
	[self setNeedsDisplay:YES];
		
    return YES;
}

- (void)removeAllFocusRings {

	NSEnumerator* enumerator = [itemsPtr objectEnumerator];
	id element;
	while ( (element = [enumerator nextObject]) )
		{
        [element setIsSelected:NO];
		for (int i=0; i<[element numInlets]; i++)
            {
			InOutlet* iolet = [element inletIndexed:i];
			[iolet setIsSelected:NO];
			}
		for (int i=0; i<[element numOutlets]; i++)
            {
			InOutlet* iolet = [element outletIndexed:i];
			[iolet setIsSelected:NO];
			}
		}
    [self setNeedsDisplay:YES];
}

- (void)removeConnectionWithOutlet:(InOutlet*)ol {

	Tool* t = [[ol partner] toolOwner];
	
	[[ol partner] setPartner:nil];
	[ol setPartner:nil];
	[t updateForConnectionChange];
}

- (void)removeSelectedConnections {

	NSEnumerator* enumerator = [itemsPtr objectEnumerator];
	id element;
	while ( (element = [enumerator nextObject]) )
        {
		// Loop through all of the outlets for this tool, looking for
		// outlets that are selected. If the outlet has been selected,
		// then remove the pointers between this outlet and its inlet. Note
		// that only outlets are selected.
		for (int i=0; i<[element numOutlets]; i++)
            {
			InOutlet* iolet = [element outletIndexed:i];
			if ([iolet isSelected] == YES)
				{
				[self removeConnectionWithOutlet:iolet];
				[iolet setIsSelected:NO];
				}
			}
        }
    [self setNeedsDisplay:YES];
    [[[NSDocumentController sharedDocumentController] currentDocument] updateChangeCount:NSChangeDone];
}

- (void)scaleFactorChanged:(NSNotification*)notification {

    if ( myScrollView != [notification object] )
        return;

	// get the old and new scale factors
    float oldFactor = scaleFactor;
    scaleFactor = [[notification object] scaleFactor];
	
	// get the index for the scale, so we can choose the correct tool image to display
	if (scaleFactor > 0.24 && scaleFactor < 0.26)
		scaleIdx = 0;
	else if (scaleFactor > 0.49 && scaleFactor < 0.51)
		scaleIdx = 1;
	else if (scaleFactor > 0.74 && scaleFactor < 0.76)
		scaleIdx = 2;
	else if (scaleFactor > 0.99 && scaleFactor < 1.01)
		scaleIdx = 3;
	else if (scaleFactor > 1.24 && scaleFactor < 1.26)
		scaleIdx = 4;
	else if (scaleFactor > 1.49 && scaleFactor < 1.51)
		scaleIdx = 5;
	else if (scaleFactor > 1.99 && scaleFactor < 2.01)
		scaleIdx = 6;
	else if (scaleFactor > 3.99 && scaleFactor < 4.01)
		scaleIdx = 7;
    
	// calculate the ratio of the scale factors to get a factor by which each
	// element in the interface will be multiplied
    float r = scaleFactor / oldFactor;

	// go through all of the tools, changing the size of each by the factor and also
	// updating the tracking information for each tool
    [analysisDocumentPtr scaleToolsByScale:scaleFactor andFactor:r];
 
    // reset the bottom right corner
    [self setCorners];
	
    // update the scroll bars
    [self updateScrollBars];

	// update the tracking information for the tools
	[self updateAllTrackingAreas];
        		
    [self setNeedsDisplay:YES];
}

- (IBAction)selectAll:(id)sender {

    AnalysisDocument* doc = [[NSDocumentController sharedDocumentController] currentDocument];
    [doc selectAllTools];
}

- (void)selectAllItems {

	NSEnumerator* enumerator = [itemsPtr objectEnumerator];
	id element;
	while ( (element = [enumerator nextObject]) )
		{
        [element setIsSelected:YES];
		for (int i=0; i<[element numOutlets]; i++)
			{
			InOutlet* ol = [element outletIndexed:i];
			[ol setIsSelected:YES];
			}
		}
    [self setNeedsDisplay:YES];
}

- (ItemSelector)selectItem:(NSPoint)forPoint {

	// set to base state
	ItemSelector mySelection;
	mySelection.selectedItem = nil;
	mySelection.selectionType = ITEM_SELECTION;
	
	// transform point to coordinate system of view objects (such as tools)
	NSPoint p = [self convertPoint:forPoint fromView:nil];
	
	// enumerate all of the tools, to see if any of them have been selected
	NSEnumerator* enumerator = [itemsPtr objectEnumerator];
	id element;
	while ( (element = [enumerator nextObject]) )
		{
		// initialize information on the tool
		NSRect toolRect, infoRect, inspRect;
		toolRect.origin = [element itemLocation];
		toolRect.size = [element itemSize];
		[self transformToBottomLeftCoordinates:(&toolRect.origin)];
        
        infoRect.size = inspRect.size = NSMakeSize(ITEM_IMAGE_SIZE*scaleFactor*0.2, ITEM_IMAGE_SIZE*scaleFactor*0.2);
        infoRect.origin = inspRect.origin = toolRect.origin;
        infoRect.origin.x += toolRect.size.width - infoRect.size.width - toolRect.size.width * 0.05;
        infoRect.origin.y += toolRect.size.height * 0.05;
        inspRect.origin.x += toolRect.size.width * 0.05;
        inspRect.origin.y += toolRect.size.height * 0.05;

		// 1, check to see if the point is in any of the inlets 
		for (int i=0; i<[element numInlets]; i++)
			{
			InOutlet* il = [element inletIndexed:i];
			NSRect r = [il getInOutletRectFromPoint:toolRect.origin];
			if ( CGRectContainsPoint( NSRectToCGRect(r), NSPointToCGPoint(p) ) )
				{
				mySelection.selectedItem = il;
				mySelection.selectionType = INLET_SELECTION;
				return mySelection;
				}
			}
			
		// 2, check to see if the point is in any of the outlets
		for (int i=0; i<[element numOutlets]; i++)
			{
			InOutlet* ol = [element outletIndexed:i];
			NSRect r = [ol getInOutletRectFromPoint:toolRect.origin];
			if ( CGRectContainsPoint( NSRectToCGRect(r), NSPointToCGPoint(p) ) )
				{
				mySelection.selectedItem = ol;
				mySelection.selectionType = OUTLET_SELECTION;
				return mySelection;
				}
			}
            
        // 3, check if the point is the information button (opening the control window)
        if ( CGRectContainsPoint( NSRectToCGRect(infoRect), NSPointToCGPoint(p) ) )
            {
            mySelection.selectedItem = element;
            mySelection.selectionType = INFO_SELECTION;
			return mySelection;
            }

        // 4, check if the point is the inspector button 
        if ([element hasInspectorInfo] == YES)
            {
            if ( CGRectContainsPoint( NSRectToCGRect(inspRect), NSPointToCGPoint(p) ) )
                {
                mySelection.selectedItem = element;
                mySelection.selectionType = INSP_SELECTION;
                return mySelection;
                }
            }
            			
		// 5, check if the point is in the tool image
		if ( CGRectContainsPoint( NSRectToCGRect(toolRect), NSPointToCGPoint(p) ) )
            {
            cursorOffset.width  = p.x - toolRect.origin.x;
            cursorOffset.height = p.y - toolRect.origin.y;
			mySelection.selectedItem = element;
			mySelection.selectionType = ITEM_SELECTION;
			return mySelection;
            }
            
        // 6, check if the point is any of the connections between objects
		for (int i=0; i<[element numOutlets]; i++)
            {
			InOutlet* ol = [element outletIndexed:i];
            if ([ol partner] != nil)
                {
				Tool* partnerTool = [[ol partner] toolOwner];
				NSPoint p1 = [ol getInOutletPointFromPoint:toolRect.origin];
				toolRect.origin = [partnerTool itemLocation];
				[self transformToBottomLeftCoordinates:(&toolRect.origin)];
				NSPoint p2 = [[ol partner] getInOutletPointFromPoint:toolRect.origin];
				
				NSPoint i1 = p1;
				NSPoint i2 = p2;
				float x = (p1.x - p2.x) * 0.5;
				if (x < 0.0)
					x = p1.x - x;
				else 
					x = p2.x + x;
				i1.x = x;
				i2.x = x;
                
                NSRect r1, r2, r3;
                r1.origin.x = MIN(p1.x, i1.x); 
                r1.origin.y = p1.y - 4.0;
                r1.size.height = 8.0; 
                r1.size.width = fabs(i1.x - p1.x);
                
                r2.origin.x = i1.x - 4.0; 
                r2.origin.y = MIN(i1.y, i2.y);
                r2.size.height = fabs(i1.y - i2.y); 
                r2.size.width = 8.0;
                
                r3.origin.x = MIN(p2.x, i2.x); 
                r3.origin.y = p2.y - 4.0;
                r3.size.height = 8.0; 
                r3.size.width = fabs(i2.x - p2.x);
                
                if ( CGRectContainsPoint(NSRectToCGRect(r1), NSPointToCGPoint(p)) || CGRectContainsPoint(NSRectToCGRect(r2), NSPointToCGPoint(p)) || CGRectContainsPoint(NSRectToCGRect(r3), NSPointToCGPoint(p)) )
                    {
					mySelection.selectedItem = ol;
					mySelection.selectionType = LINK_SELECTION;
					return mySelection;
                    }
               }
            }
			
		}
		
	return mySelection;
}

- (void)selectItemsInSweepArea {

    // set up the rectangle for the sweep area
    NSRect sweepRect;
    NSPoint p1 = sweepStartLocation;
    NSPoint p2 = sweepCurrentLocation;
    sweepRect.origin = p1;
    if (p2.x < p1.x)
        sweepRect.origin.x = p2.x;
    if (p2.y < p1.y)
        sweepRect.origin.y = p2.y;
    sweepRect.size.width = p1.x - p2.x;
    sweepRect.size.height = p1.y - p2.y;
    if (sweepRect.size.width < 0.0)
        sweepRect.size.width = -sweepRect.size.width;
    if (sweepRect.size.height < 0.0)
        sweepRect.size.height = -sweepRect.size.height;
        
    // enumerate the tools, deciding if any of them are in the sweep rectangle
	NSEnumerator* enumerator = [itemsPtr objectEnumerator];
	id element;
	while ( (element = [enumerator nextObject]) )
		{
        // get a rectangle for the tool
		NSRect toolRect;
		toolRect.origin = [element itemLocation];
		toolRect.size   = [element itemSize];
		[self transformToBottomLeftCoordinates:(&toolRect.origin)];
        
        // decide if the tool's rectangle is inside the sweep rectangle
		if ( CGRectIntersectsRect( NSRectToCGRect(sweepRect), NSRectToCGRect(toolRect) ) )
            {
            if ( [selectedItems containsObject:element] == NO )
                [element setIsSelected:YES];
            }
        else 
            {
            if ( [element isSelected] == YES && [selectedItems containsObject:element] == NO )
                [element setIsSelected:NO];
            }
            
        // decide if any of the tool's connections are in the sweep rectangle
		for (int i=0; i<[element numOutlets]; i++)
			{
			InOutlet* ol = [element outletIndexed:i];
			if ([ol partner] != nil)
				{
                // get the location of the tool's outlet
				NSPoint p1 = [ol getInOutletPointFromPoint:toolRect.origin];
                
                // get the location of the partner's inlet
				NSRect partnerToolRect;
				partnerToolRect.origin = [[[ol partner] toolOwner] itemLocation];
				[self transformToBottomLeftCoordinates:(&partnerToolRect.origin)];
				NSPoint p2 = [[ol partner] getInOutletPointFromPoint:partnerToolRect.origin];
				
                // set up three rectangles for the kinked connection between the tools
                NSRect r1, r2, r3;
				NSPoint i1 = p1;
				NSPoint i2 = p2;
				float x = (p1.x - p2.x) * 0.5;
				if (x < 0.0)
					x = p1.x - x;
				else 
					x = p2.x + x;
				i1.x = x;
				i2.x = x;
                
                if (p1.x < i1.x)
                    r1.origin.x = p1.x;
                else 
                    r1.origin.x = i1.x;
                r1.origin.y = p1.y - 2.0;
                r1.size.height = 4.0;
                r1.size.width = fabs(p1.x - i1.x);

                if (p2.x < i2.x)
                    r2.origin.x = p2.x;
                else 
                    r2.origin.x = i2.x;
                r2.origin.y = p2.y - 2.0;
                r2.size.height = 4.0;
                r2.size.width = fabs(p2.x - i2.x);
                
                if (i1.y < i2.y)
                    r3.origin.y = i1.y;
                else 
                    r3.origin.y = i2.y;
                r3.origin.x = i1.x - 2.0;
                r3.size.width = 4.0;
                r3.size.height = fabs(i1.y - i2.y);

                if ( CGRectIntersectsRect(NSRectToCGRect(sweepRect), NSRectToCGRect(r1)) ||
                     CGRectIntersectsRect(NSRectToCGRect(sweepRect), NSRectToCGRect(r2)) ||
                     CGRectIntersectsRect(NSRectToCGRect(sweepRect), NSRectToCGRect(r3)) )
                    {
                    if ( [selectedItems containsObject:ol] == NO )
                        [ol setIsSelected:YES];
                    }
                else 
                    {
                    if ( [ol isSelected] == YES && [selectedItems containsObject:ol] == NO )
                        [ol setIsSelected:NO];
                    }

				}
			}
        }
        
    // update the view to reflect the possible selections
    [self setNeedsDisplay:YES];
}

- (void)updateBackgroundColor:(NSNotification*)notification {

    bkgrndColor = [[notification object] analysisBkgrndColor];
    gridColor   = [[notification object] analysisGridColor];
    [self setNeedsDisplay:YES];
}

- (void)writeToPasteboard:(NSPasteboard*)pb {

	// remove connections between selected and unselected tools so that
	// the archiving of the copied/cut tools goes OK
	NSMutableArray* itemPairs = [NSMutableArray arrayWithCapacity:1];
	NSEnumerator* itemEnumerator = [itemsPtr objectEnumerator];
	id element;
	while ( (element = [itemEnumerator nextObject]) )
		{
		for (int i=0; i<[element numOutlets]; i++)
			{
			InOutlet* ol = [element outletIndexed:i];
			RbItem* t1 = [ol toolOwner];
			RbItem* t2 = [[ol partner] toolOwner];
			if ( [t1 isSelected] == YES && [t2 isSelected] == YES && [ol isSelected] == NO )
				{
				ConnectionPair* p = [[ConnectionPair alloc] init];
				[p setItem1:ol];
				[p setItem2:[ol partner]];
				[itemPairs addObject:p];
				[[ol partner] setPartner:nil];
				[ol setPartner:nil];
				}
			else if ( ([t1 isSelected] == YES && [t2 isSelected] == NO) || ([t1 isSelected] == NO && [t2 isSelected] == YES)  )
				{
				ConnectionPair* p = [[ConnectionPair alloc] init];
				[p setItem1:ol];
				[p setItem2:[ol partner]];
				[itemPairs addObject:p];
				[[ol partner] setPartner:nil];
				[ol setPartner:nil];
				}
			}
		}
		
    // get a list of the selected items to archive
    [copiedItems removeAllObjects];
	itemEnumerator = [itemsPtr objectEnumerator];
	while ( (element = [itemEnumerator nextObject]) )
		{
        if ([element isSelected] == YES)
            [copiedItems addObject:element];
        }
        
    // archive the data to the pasteboard
    NSData* d = [NSKeyedArchiver archivedDataWithRootObject:copiedItems];
    [pb setData:d forType:ToolDataPboardType];
	
	// re-establish any removed connections
	NSEnumerator* connectionEnumerator = [itemPairs objectEnumerator];
	while ( (element = [connectionEnumerator nextObject]) )
		{
		InOutlet* ol1 = [element item1];
		InOutlet* ol2 = [element item2];
		[ol1 setPartner:ol2];
		[ol2 setPartner:ol1];
		}
}

@end